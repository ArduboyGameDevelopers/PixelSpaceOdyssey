#include "game.h"
#include "bridge.h"

#include "Arduboy.h"
#include "graphics.h"

#include "animation.h"
#include "spider.h"
#include "tiles.h"

#include "player.h"
#include "spider.h"

static TimeInterval lastFrameTime;

static Arduboy display;
static Graphics graphics = Graphics(display.getBuffer(), WIDTH, HEIGHT);

static Player player;
static Spider spider;

short camX, camY;
short camXMax, camYMax;

inline bool button_pressed(uint8_t button)
{
    return display.pressed(button);
}

void updateInput();
void drawTileMap();
void updateSpider();

void startGame()
{
    display.setFrameRate(10);
    display.start();
    
    player.x = SCREEN_TO_WORLD(8);
    player.y = SCREEN_TO_WORLD(40);
    spider.x = SCREEN_TO_WORLD(86);
    spider.y = SCREEN_TO_WORLD(80);
    spider.setState(SPIDER_SLEEP);
    
    camX = 0;
    camY = 20;
    camXMax = MAP_WIDTH * 8 - 128;
    camYMax = MAP_HEIGHT * 8 - 64;
    
    lastFrameTime = millis();
}

void loopGame()
{
    TimeInterval dt = millis() - lastFrameTime;
    lastFrameTime = millis();
    
    updateInput();
    
    player.update(dt);
    spider.update(dt);
    
    display.fillRect(0, 0, 128, 64, WHITE);
    
    graphics.translate(-camX, -camY);
    
    drawTileMap();
    player.draw(&graphics);
    spider.draw(&graphics);
    
    graphics.translate(camX, camY);
    
    display.display();
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
        if (spider.state() == SPIDER_SLEEP)
        {
            spider.setState(SPIDER_AWAKEN);
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

void drawTileMap()
{
    int16_t x = 0, y = 0;
    uint16_t index = 0;
    for (int ty = 0; ty < MAP_HEIGHT; ++ty)
    {
        for (int tx = 0; tx < MAP_WIDTH; ++tx)
        {
            int tileIndex = indices[index];
            graphics.drawImage(tiles[tileIndex], x, y, 8, 8, DM_UNLIT);
            x += 8;
            ++index;
        }
        x = 0;
        y += 8;
    }
}