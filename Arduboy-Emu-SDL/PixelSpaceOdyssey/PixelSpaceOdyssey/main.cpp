#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "DisplayView.h"

#include "game.h"
#include "platform.h"
#include "bridge.h"

static const int SCREEN_WIDTH  = 512;
static const int SCREEN_HEIGHT = 256;

static DisplayView* displayView;

static const uint8_t LEFT_BUTTON  = 1 << 5;
static const uint8_t RIGHT_BUTTON = 1 << 2;
static const uint8_t UP_BUTTON    = 1 << 4;
static const uint8_t DOWN_BUTTON  = 1 << 6;
static const uint8_t A_BUTTON     = 1 << 1;
static const uint8_t B_BUTTON     = 1 << 0;
static int inputMask = 0;

void handleEvent(const SDL_Event* event);

int main(int argc, char * argv[])
{
    // Initialize SDL with video
    SDL_Init(SDL_INIT_VIDEO);
    
    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Pixel Space Odyssey",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_OPENGL);
    
    // if failed to create a window
    if(!window)
    {
        // we'll print an error message and exit
        std::cerr << "Error failed to create window!\n";
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // if we failed to create a context
    if(!renderer)
    {
        // we'll print out an error message and exit
        std::cerr << "Error failed to create a renderer\n!";
        return 2;
    }
    
    // init display view
    displayView = new DisplayView(renderer);
    
    SDL_Event event;	 // used to store any events from the OS
    bool running = true; // used to determine if we're running the game
    
    startGame();
    
    Uint32 frameDelay = 1000/30;
    Uint32 lastFrameTime = 0;
    
    while(running)
    {
        // poll for events from SDL
        while(SDL_PollEvent(&event))
        {
            // determine if the user still wants to have the window open
            // (this basically checks if the user has pressed 'X')
            running = event.type != SDL_QUIT;
            handleEvent(&event);
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        if (SDL_GetTicks() - lastFrameTime >= frameDelay)
        {
            lastFrameTime = SDL_GetTicks();
            updateGame();
        }
        
        drawGame();
        
        // Swap OpenGL buffers
        SDL_RenderPresent(renderer);
    }
    
    // Delete display view
    delete displayView;
    
    // Destroy the renderer
    SDL_DestroyRenderer(renderer);
    
    // Destroy the window
    SDL_DestroyWindow(window);
    
    // And quit SDL
    SDL_Quit();
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// Events

int keyButtonMask(SDL_Keycode code)
{
    switch (code)
    {
        case SDLK_w: return UP_BUTTON;
        case SDLK_a: return LEFT_BUTTON;
        case SDLK_s: return DOWN_BUTTON;
        case SDLK_d: return RIGHT_BUTTON;
        case SDLK_n: return A_BUTTON;
        case SDLK_m: return B_BUTTON;
    }
    
    return 0;
}

void handleKeyboardEvent(const SDL_KeyboardEvent* event)
{
    int keyMask = keyButtonMask(event->keysym.sym);
    if (keyMask)
    {
        if (event->type == SDL_KEYDOWN)
        {
            inputMask |= keyMask;
        }
        else if (event->type == SDL_KEYUP)
        {
            inputMask &= ~keyMask;
        }
    }
}

void handleEvent(const SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            handleKeyboardEvent(&event->key);
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Platform

void platformRenderScreen(unsigned const char* screenBuffer, int width, int height)
{
    displayView->renderScreenBuffer(screenBuffer, width, height);
}

unsigned long platformMillis(void)
{
    return SDL_GetTicks();
}

void platformDelay(unsigned long millis)
{
    SDL_Delay((Uint32)millis);
}

uint8_t platformInput()
{
    return inputMask;
}