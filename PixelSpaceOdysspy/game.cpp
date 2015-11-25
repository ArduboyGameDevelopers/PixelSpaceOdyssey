#include "game.h"
#include "bridge.h"

#include "Arduboy.h"
#include "animation.h"
#include "images.h"
#include "tiles.h"

Arduboy display;

int frame = 0;
int animation = 0;

short x, y;
bool jumping;
bool crouching;
unsigned short camX, camY;

short jump_speed;
signed char move_dir;

inline bool button_pressed(uint8_t button)
{
    return display.pressed(button);
}

void set_animation(uint8_t anim)
{
    if (animation != anim)
    {
        animation = anim;
        frame = 0;
    }
}

void updateInput();
void updatePlayer();
void updateAnimation();
void drawTileMap();
void drawPlayer();
void drawBitmapFlipped(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color, int8_t flip);

void startGame()
{
    display.setFrameRate(10);
    display.start();
    
    x = SCREEN_TO_WORLD(8);
    y = SCREEN_TO_WORLD(40);

    jump_speed = 0;
    jumping = false;
    crouching = false;
    move_dir = 0;
}

void loopGame()
{
    if (!display.nextFrame()) return;
    
    updateInput();
    updatePlayer();
    updateAnimation();
    
    display.fillRect(0, 0, 128, 64, WHITE);

    drawTileMap();
    drawPlayer();
}

void updateInput()
{
    crouching = button_pressed(DOWN_BUTTON);
    
    if (!jumping && button_pressed(JUMP_BUTTON))
    {
        jumping = true;
        jump_speed = JUMP_SPEED;
        set_animation(ANIMATION_JUMP);
    }
    
    if (!jumping)
    {
        move_dir = 0;
        
        if (!crouching)
        {
            if (button_pressed(LEFT_BUTTON)) move_dir = -1;
            else if (button_pressed(RIGHT_BUTTON)) move_dir = 1;
            if (button_pressed(RUN_BUTTON))
            {
                move_dir = move_dir + move_dir; // avoid multiplication - save a tree
            }
        }
    }
}

void updatePlayer()
{
    x += move_dir * WALK_SPEED;
    if (x < MIN_X) x = MIN_X;
    else if (x > MAX_X) x = MAX_X;
    
    if (jumping)
    {
        jump_speed += GRAVITY;
        y += jump_speed;
        
        if (y > FLOOR)
        {
            y = FLOOR;
            jump_speed = 0;
            jumping = false;
        }
    }
}

void updateAnimation()
{
    if (jumping)
    {
        set_animation(ANIMATION_JUMP);
    }
    else if (crouching)
    {
        set_animation(ANIMATION_CROUCH);
    }
    else if (move_dir == 0)
    {
        set_animation(ANIMATION_STAT_1);
    }
    else if (move_dir == 1 || move_dir == -1)
    {
        set_animation(ANIMATION_WALK);
    }
    else
    {
        set_animation(ANIMATION_RUN);
    }
}

void draw_animation(FrameData data, uint8_t x, uint8_t y)
{
    uint8_t offsetX = pgm_read_byte(data);
    uint8_t offsetY = pgm_read_byte(data + 1);
    uint8_t width   = pgm_read_byte(data + 2);
    uint8_t height  = pgm_read_byte(data + 3);
    
    FrameData ptr = data + 4;
    drawBitmapFlipped(x + offsetX, y + offsetY, ptr, width, height, BLACK, move_dir < 0 ? 1 : 0);
    display.display();
}

void drawTileMap()
{
    uint8_t x = 0, y = 0;
    uint16_t index = 0;
    for (int ty = 0; ty < MAP_HEIGHT; ++ty)
    {
        for (int tx = 0; tx < MAP_WIDTH; ++tx)
        {
            int tileIndex = indices[index];
            drawBitmapFlipped(x, y, tiles[tileIndex], 8, 8, BLACK, 0);
            x += 8;
            ++index;
        }
        x = 0;
        y += 8;
    }
}

void drawPlayer()
{
    uint8_t draw_x = WORLD_TO_SCREEN(x) - 8;
    uint8_t draw_y = WORLD_TO_SCREEN(y) - 16;
    
    draw_animation(animations[animation].frames[frame], draw_x, draw_y);
    
    ++frame;
    if (frame >= animations[animation].frameCount)
    {
        frame = 0;
    }
}

void drawBitmapFlipped(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color, int8_t flip)
{
    // no need to dar at all of we're offscreen
    if (x+w < 0 || x > WIDTH-1 || y+h < 0 || y > HEIGHT-1)
        return;
    
    unsigned char* sBuffer = display.getBuffer();
    
    int yOffset = abs(y) % 8;
    int sRow = y / 8;
    if (y < 0)
    {
        sRow--;
        yOffset = 8 - yOffset;
    }
    int rows = h/8;
    if (h%8!=0) rows++;
    
    int8_t k1 = flip;
    int8_t k2 = flip > 0 ? -1 : 1;
    
    for (int a = 0; a < rows; a++)
    {
        int bRow = sRow + a;
        if (bRow > (HEIGHT/8)-1) break;
        if (bRow > -2)
        {
            for (int iCol = 0; iCol<w; iCol++)
            {
                if (iCol + x > (WIDTH-1)) break;
                if (iCol + x >= 0)
                {
                    if (bRow >= 0)
                    {
                        if      (color == WHITE) sBuffer[ (bRow*WIDTH) + k1 * (w - 1) + x + k2 * iCol ] |= pgm_read_byte(bitmap+(a*w)+iCol) << yOffset;
                        else if (color == BLACK) sBuffer[ (bRow*WIDTH) + k1 * (w - 1) + x + k2 * iCol ] &= ~(pgm_read_byte(bitmap+(a*w)+iCol) << yOffset);
                        else                     sBuffer[ (bRow*WIDTH) + k1 * (w - 1) + x + k2 * iCol ] ^= pgm_read_byte(bitmap+(a*w)+iCol) << yOffset;
                    }
                    if (yOffset && bRow<(HEIGHT/8)-1 && bRow > -2)
                    {
                        if      (color == WHITE) sBuffer[ ((bRow+1)*WIDTH) + k1 * (w - 1) + x + k2 * iCol ] |= pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset);
                        else if (color == BLACK) sBuffer[ ((bRow+1)*WIDTH) + k1 * (w - 1) + x + k2 * iCol ] &= ~(pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset));
                        else                     sBuffer[ ((bRow+1)*WIDTH) + k1 * (w - 1) + x + k2 * iCol ] ^= pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset);
                    }
                }
            }
        }
    }
}
