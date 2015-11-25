#include "game.h"
#include "bridge.h"

#include "Arduboy.h"
#include "animation.h"
#include "images.h"
#include "spider.h"
#include "tiles.h"

Arduboy display;

int frame = 0;
int animation = 0;

short x, y;
bool jumping;
bool crouching;
short camX, camY;
short camXMax, camYMax;

short jump_speed;
signed char move_dir;

#define SPIDER_SLEEP 0
#define SPIDER_AWAKEN 1
#define SPIDER_RISE 2
#define SPIDER_WALK 3

typedef uint8_t SpiderState;

short spiderX, spiderY;
int8_t spiderMovement;
SpiderState spiderState;
uint8_t spiderAnimation;
uint8_t spiderFrame;
bool spiderAnimationLoop;

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

void setSpiderState(SpiderState state);

void updateInput();
void updatePlayer();
void updateAnimation();
void drawTileMap();
void drawPlayer();
void updateSpider();
void drawSpider();
void drawBitmapFlipped(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color, int8_t flip);

void startGame()
{
    display.setFrameRate(10);
    display.start();
    
    x = SCREEN_TO_WORLD(8);
    y = SCREEN_TO_WORLD(40);
    
    camX = 0;
    camY = 20;
    camXMax = MAP_WIDTH * 8 - 128;
    camYMax = MAP_HEIGHT * 8 - 64;

    jump_speed = 0;
    jumping = false;
    crouching = false;
    move_dir = 0;
    
    spiderX = 86;
    spiderY = 80;
    setSpiderState(SPIDER_SLEEP);
}

void loopGame()
{
    if (!display.nextFrame()) return;
    
    updateInput();
    updatePlayer();
    updateSpider();
    updateAnimation();
    
    display.fillRect(0, 0, 128, 64, WHITE);
    
    drawTileMap();
    drawPlayer();
    drawSpider();
    
    display.display();
}

void setSpiderAnimation(int animation)
{
    spiderAnimation = animation;
    spiderFrame = 0;
}

void setSpiderState(SpiderState state)
{
    spiderAnimationLoop = false;
    spiderState = state;
    
    switch (state)
    {
        case SPIDER_SLEEP:
            spiderAnimationLoop = true;
            setSpiderAnimation(SPIDER_ANIMATION_SLEEP);
            break;
        case SPIDER_AWAKEN:
            setSpiderAnimation(SPIDER_ANIMATION_AWAKEN);
            break;
        case SPIDER_RISE:
            setSpiderAnimation(SPIDER_ANIMATION_RISE);
            break;
        case SPIDER_WALK:
            spiderAnimationLoop = true;
            setSpiderAnimation(SPIDER_ANIMATION_WALK);
            break;
    }
}

void updateInput()
{
    if (button_pressed(LEFT_BUTTON))
    {
        camX -= 2;
        if (camX < 0) camX = 0;
    }
    else if (button_pressed(RIGHT_BUTTON))
    {
        camX += 2;
        if (camX > camXMax) camX = camXMax;
    }
    
    if (button_pressed(UP_BUTTON))
    {
        camY -= 2;
        if (camY < 0) camY = 0;
    }
    else if (button_pressed(DOWN_BUTTON))
    {
        camY += 2;
        if (camY > camYMax) camY = camYMax;
    }
    
    if (button_pressed(A_BUTTON) || button_pressed(B_BUTTON))
    {
        if (spiderState == SPIDER_SLEEP)
        {
            setSpiderState(SPIDER_AWAKEN);
        }
    }
    
    /*
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
    */
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

void drawAnimation(FrameData data, int16_t x, int16_t y)
{
    uint8_t offsetX = pgm_read_byte(data);
    uint8_t offsetY = pgm_read_byte(data + 1);
    uint8_t width   = pgm_read_byte(data + 2);
    uint8_t height  = pgm_read_byte(data + 3);
    
    FrameData ptr = data + 4;
    drawBitmapFlipped(x + offsetX, y + offsetY, ptr, width, height, BLACK, move_dir < 0 ? 1 : 0);
}

void drawTileMap()
{
    int16_t x = -camX, y = -camY;
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
        x = -camX;
        y += 8;
    }
}

void drawPlayer()
{
    int16_t draw_x = WORLD_TO_SCREEN(x) - 8 - camX;
    int16_t draw_y = WORLD_TO_SCREEN(y) - 16 - camY;
    
    drawAnimation(animations[animation].frames[frame], draw_x, draw_y);
    
    ++frame;
    if (frame >= animations[animation].frameCount)
    {
        frame = 0;
    }
}

void drawSpider()
{
    int16_t drawX = spiderX - camX - 12;
    int16_t drawY = spiderY - camY - 16;
    
    drawAnimation(spider_animations[spiderAnimation].frames[spiderFrame], drawX, drawY);
}

void updateSpider()
{
    ++spiderFrame;
    if (spiderFrame >= spider_animations[spiderAnimation].frameCount)
    {
        if (spiderAnimationLoop)
        {
            spiderFrame = 0;
        }
        else
        {
            setSpiderState(spiderState + 1);
        }
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
