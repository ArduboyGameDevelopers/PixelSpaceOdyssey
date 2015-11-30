#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "DisplayView.h"
#include "TileView.h"

#include "game.h"
#include "platform.h"
#include "bridge.h"

static const int GAME_WIDTH   = 512;
static const int GAME_HEIGHT  = 256;
static const int TILES_WIDTH  = 512;
static const int TILES_HEIGHT = 5 * (32 + 2) + 2;

static const int SCREEN_WIDTH  = GAME_WIDTH;
static const int SCREEN_HEIGHT = GAME_HEIGHT + TILES_HEIGHT;

static DisplayView* displayView;
static TileView* tileView;

static const int kViewIndexDisplayView  = 0;
static const int kViewIndexTileView     = 1;
static const int kViewsCount            = 2;
static View* views[kViewsCount];

static const uint8_t LEFT_BUTTON  = 1 << 5;
static const uint8_t RIGHT_BUTTON = 1 << 2;
static const uint8_t UP_BUTTON    = 1 << 4;
static const uint8_t DOWN_BUTTON  = 1 << 6;
static const uint8_t A_BUTTON     = 1 << 1;
static const uint8_t B_BUTTON     = 1 << 0;
static int inputMask = 0;

static unsigned long gameMillis;

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
    displayView = new DisplayView(GAME_WIDTH, GAME_HEIGHT);
    views[kViewIndexDisplayView] = displayView;
    
    // load tiles
    SDL_Surface *tilesSurface = SDL_LoadBMP("tiles.bmp");
    SDL_Texture *tilesTexture = SDL_CreateTextureFromSurface(renderer, tilesSurface);
    tileView = new TileView(tilesTexture, TILES_WIDTH, TILES_HEIGHT);
    tileView->setPos(0, displayView->bottom());
    views[kViewIndexTileView] = tileView;
    SDL_FreeSurface(tilesSurface);
    
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
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            
            handleEvent(&event);
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        Uint32 delta = SDL_GetTicks() - lastFrameTime;
        if (delta >= frameDelay)
        {
            lastFrameTime = SDL_GetTicks();
            gameMillis += delta;
            
            updateGame();
            drawGame();
        }
        
        displayView->render(renderer);
        
        // draw tiles
        tileView->render(renderer);
        
        // Swap OpenGL buffers
        SDL_RenderPresent(renderer);
    }
    
    // Delete display view
    delete displayView;
    
    // Delete tiles view
    delete tileView;
    
    // Destroy tiles texture
    SDL_DestroyTexture(tilesTexture);
    
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

View* findMouseView(int x, int y);

int keyButtonMask(SDL_Keycode code)
{
    switch (code)
    {
        case SDLK_w:
        case SDLK_UP:
            return UP_BUTTON;
            
        case SDLK_a:
        case SDLK_LEFT:
            return LEFT_BUTTON;
            
        case SDLK_s:
        case SDLK_DOWN:
            return DOWN_BUTTON;
            
        case SDLK_d:
        case SDLK_RIGHT:
            return RIGHT_BUTTON;
            
        case SDLK_n:
        case SDLK_LALT:
            return A_BUTTON;
            
        case SDLK_m:
        case SDLK_SPACE:
            return B_BUTTON;
    }
    
    return 0;
}

void handleKeyboardEvent(const SDL_KeyboardEvent* event)
{
    SDL_Keysym keysym = event->keysym;
    
    int keyMask = keyButtonMask(keysym.sym);
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
    
    if (event->type == SDL_KEYDOWN)
    {
        switch (keysym.sym)
        {
            case SDLK_g:
                displayView->toggleGrid();
                break;
        }
    }
}

void handleMouseMotionEvent(const SDL_MouseMotionEvent* event)
{
}

void handleMouseButtonEvent(const SDL_MouseButtonEvent* event)
{
    int x = event->x;
    int y = event->y;
    
    View *view = findMouseView(x, y);
    if (view)
    {
        int localX = x - view->left();
        int localY = y - view->top();
        
        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            view->mouseDown(localX, localY);
        }
        else if (event->type == SDL_MOUSEBUTTONUP)
        {
            view->mouseUp(localX, localY);
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
            
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            handleMouseButtonEvent(&event->button);
            break;
            
        case SDL_MOUSEMOTION:
            handleMouseMotionEvent(&event->motion);
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Mouse

View* findMouseView(int x, int y)
{
    for (int i = 0; i < kViewsCount; ++i)
    {
        View* view = views[i];
        if (view->containsPoint(x, y))
        {
            return view;
        }
    }
    
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////
// Platform

void platformRenderScreen(unsigned const char* screenBuffer, int width, int height)
{
    displayView->copyScreenBuffer(screenBuffer, width, height);
}

unsigned long platformMillis(void)
{
    return gameMillis;
}

void platformDelay(unsigned long millis)
{
    SDL_Delay((Uint32)millis);
}

uint8_t platformInput()
{
    return inputMask;
}