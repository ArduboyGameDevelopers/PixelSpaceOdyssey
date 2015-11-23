#include "game.h"

#include "Arduboy.h"
#include "animation.h"
#include "images.h"

Arduboy display;

extern void DrawFrame(FrameData data, uint8_t x, uint8_t y);

int frame = 0;
int animation = 0;

void startGame()
{
    display.setFrameRate(10);
    display.start();
}

void loopGame()
{
    if (!display.nextFrame()) return;
    
    display.clearDisplay();
    display.fillRect(0, 0, 128, 48, WHITE);
    
    int x = (128-16) / 2;
    int y = (64-12) / 2;
    
    if (display.pressed(LEFT_BUTTON)) x -= 32;
    else if (display.pressed(RIGHT_BUTTON)) x += 32;
    else if (display.pressed(UP_BUTTON)) y -= 16;
    else if (display.pressed(DOWN_BUTTON)) y += 8;
    
    DrawFrame(animations[animation].frames[frame], x, y);
    
    ++frame;
    if (frame >= animations[animation].frameCount)
    {
        frame = 0;
        animation = (animation + 1) % ANIMATIONS_COUNT;
    }
}

void DrawFrame(FrameData data, uint8_t x, uint8_t y)
{
    uint8_t offsetX = pgm_read_byte(data);
    uint8_t offsetY = pgm_read_byte(data + 1);
    uint8_t width   = pgm_read_byte(data + 2);
    uint8_t height  = pgm_read_byte(data + 3);
    
    FrameData ptr = data + 4;
    display.drawBitmap(x + offsetX, y + offsetY, ptr, width, height, BLACK);
    display.display();
}

#ifdef EMULATOR

unsigned char* getDisplayBuffer(int* buffer_size)
{
    *buffer_size = WIDTH * HEIGHT / 8;
    return display.getBuffer();
}

int getDisplayWidth()
{
    return WIDTH;
}

int getDisplayHeight()
{
    return HEIGHT;
}

#endif
