#include "game.h"
#include "bridge.h"

#include "Arduboy.h"
#include "graphics.h"

#include "animation.h"
#include "spider.h"
#include "tiles.h"

#include "player.h"
#include "spider.h"
#include "tiles.h"
#include "tilemap.h"

static TimeInterval lastFrameTime;

static Arduboy display;
static Graphics graphics = Graphics(display.getBuffer(), WIDTH, HEIGHT);

static Player player;
static Spider spider;
static TileMap tileMap = level1Map();

short camX, camY;
short camXMax, camYMax;

inline bool button_pressed(uint8_t button)
{
    return display.pressed(button);
}

void updateInput();

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
    camXMax = tileMap.pixelWidth() - 128;
    camYMax = tileMap.pixelHeight() - 64;
    
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

    Graphics *g = &graphics;
    
    tileMap.draw(g, 0, 0);
    player.draw(g);
    spider.draw(g);
    
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