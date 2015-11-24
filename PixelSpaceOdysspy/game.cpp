#include "game.h"

#include "Arduboy.h"
#include "animation.h"
#include "images.h"

Arduboy display;

#define RUN_BUTTON A_BUTTON
#define JUMP_BUTTON B_BUTTON

#define WORLD_TO_SCREEN(X) ((X) >> 4)
#define SCREEN_TO_WORLD(X) ((X) << 4)

#define GRAVITY 50
#define FLOOR 416

#define JUMP_SPEED -200
#define WALK_SPEED 32

#define MIN_X 0
#define MAX_X 1824

int frame = 0;
int animation = 0;

short x, y;
short jump_speed;
signed char move_dir;
bool jumping;
bool crouching;

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

void update_input();
void update_player();
void update_animation();
void draw_player();

void startGame()
{
    display.setFrameRate(10);
    display.start();
    
    x = SCREEN_TO_WORLD((128-16) / 2);
    y = FLOOR;

    jump_speed = 0;
    jumping = false;
    crouching = false;
    move_dir = 0;
}

void loopGame()
{
    if (!display.nextFrame()) return;
    
    update_input();
    update_player();
    update_animation();
    
    display.clearDisplay();
    display.fillRect(0, 0, 128, 48, WHITE);
    
    draw_player();
}

void update_input()
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

void update_player()
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

void update_animation()
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
    display.drawBitmap(x + offsetX, y + offsetY, ptr, width, height, BLACK);
    display.display();
}

void draw_player()
{
    uint8_t draw_x = WORLD_TO_SCREEN(x);
    uint8_t draw_y = WORLD_TO_SCREEN(y);
    
    draw_animation(animations[animation].frames[frame], draw_x, draw_y);
    
    ++frame;
    if (frame >= animations[animation].frameCount)
    {
        frame = 0;
    }
}
