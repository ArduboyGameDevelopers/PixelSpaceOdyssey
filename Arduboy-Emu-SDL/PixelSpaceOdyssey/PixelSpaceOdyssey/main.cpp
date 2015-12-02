#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Texture.h"
#include "Image.h"
#include "Button.h"

#include "RootView.h"
#include "TileSet.h"
#include "DisplayView.h"
#include "TileView.h"
#include "RectView.h"

#include "game.h"
#include "platform.h"
#include "bridge.h"

static const int kButtonViewHeight  = 31;

static const int kDisplayViewWidth  = 512;
static const int kDisplayViewHeight = 256;

static const int kTilesViewWidth    = kDisplayViewWidth;
static const int kTilesViewHeight   = 5 * (32 + 2) + 2;

static const int kWindowWidth       = kDisplayViewWidth;
static const int kWindowHeight      = kButtonViewHeight + kDisplayViewHeight + kTilesViewHeight;

static RootView* rootView;
static DisplayView* displayView;

static const uint8_t LEFT_BUTTON  = 1 << 5;
static const uint8_t RIGHT_BUTTON = 1 << 2;
static const uint8_t UP_BUTTON    = 1 << 4;
static const uint8_t DOWN_BUTTON  = 1 << 6;
static const uint8_t A_BUTTON     = 1 << 1;
static const uint8_t B_BUTTON     = 1 << 0;
static int inputMask = 0;

static unsigned long gameMillis;

void handleEvent(const SDL_Event* event);

void createButtons(SDL_Renderer* renderer);
Button *createButton(SDL_Renderer* renderer, const char* filename, ButtonClickHandler handler);

void gridButtonPressed(Button* button);
void playButtonPressed(Button* button);
void pauseButtonPressed(Button* button);
void stepButtonPressed(Button* button);

int main(int argc, char * argv[])
{
    // Initialize SDL with video
    SDL_Init(SDL_INIT_VIDEO);
    
    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Pixel Space Odyssey",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          kWindowWidth, kWindowHeight,
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
    
    // Create UI
    rootView = new RootView(kWindowWidth, kWindowHeight);
    
    // UI: buttons
    createButtons(renderer);
    
    // UI: display view
    displayView = new DisplayView(kDisplayViewWidth, kDisplayViewHeight);
    displayView->setPos(0, kButtonViewHeight);
    rootView->addView(displayView);
    
    // UI: tiles
    SDL_Surface *tilesSurface = SDL_LoadBMP("tiles.bmp");
    SDL_Texture *tilesTexture = SDL_CreateTextureFromSurface(renderer, tilesSurface);
    TileSet tileSet(tilesTexture);
    SDL_FreeSurface(tilesSurface);
    
    TileView* tileView = new TileView(tilesTexture, kTilesViewWidth, kTilesViewHeight);
    tileView->setPos(0, displayView->bottom());
    rootView->addView(tileView);
    
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
        
        Uint32 ticks = SDL_GetTicks();
        Uint32 delta = ticks - lastFrameTime;
        if (delta >= frameDelay)
        {
            lastFrameTime = ticks;
            gameMillis += delta;
            
            updateGame();
            drawGame();
        }
        
        // Update UI
        rootView->update(ticks);
        
        // Draw UI
        rootView->render(renderer);
        
        // Swap OpenGL buffers
        SDL_RenderPresent(renderer);
    }
    
    // Delete root view
    rootView->release();
    
    // Delete display view
    displayView->release();
    
    // Delete tiles view
    tileView->release();
    
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
    int x = event->x;
    int y = event->y;
    
    rootView->mouseMove(x, y);
}

void handleMouseButtonEvent(const SDL_MouseButtonEvent* event)
{
    int x = event->x;
    int y = event->y;
    
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        rootView->mouseDown(x, y);
    }
    else if (event->type == SDL_MOUSEBUTTONUP)
    {
        rootView->mouseUp(x, y);
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
// Buttons

void createButtons(SDL_Renderer* renderer)
{
    RectView* rect = new RectView(kWindowWidth, kButtonViewHeight, 234, 234, 234);
    rootView->addView(rect);
    rect->release();
    
    Button* buttonGrid = createButton(renderer, "button_grid.bmp", gridButtonPressed);
    rootView->addView(buttonGrid);
    buttonGrid->release();
    
    Button* buttonPlay = createButton(renderer, "button_play.bmp", playButtonPressed);
    rootView->addView(buttonPlay);
    buttonPlay->release();
    
    Button* buttonPause = createButton(renderer, "button_pause.bmp", pauseButtonPressed);
    rootView->addView(buttonPause);
    buttonPause->release();
    
    Button* buttonStep = createButton(renderer, "button_step.bmp", stepButtonPressed);
    rootView->addView(buttonStep);
    buttonStep->release();
    
    int totalWidth = buttonPlay->width() + buttonPause->width() + buttonStep->width();
    int buttonX = (kWindowWidth - totalWidth) / 2;
    int buttonY = (kButtonViewHeight - buttonPlay->height()) / 2;
    buttonGrid->setPos(buttonY, buttonY);
    buttonPlay->setPos(buttonX, buttonY); buttonX += buttonPlay->width();
    buttonPause->setPos(buttonX, buttonY); buttonX += buttonPause->width();
    buttonStep->setPos(buttonX, buttonY);
}

Button *createButton(SDL_Renderer* renderer, const char* filename, ButtonClickHandler handler)
{
    Texture* texture = Texture::load(renderer, filename);
    
    assert(texture->width() % kButtonStateCount == 0);
    int width = texture->width() / kButtonStateCount;
    int height = texture->height();

    Image* normalImage = new Image(texture, 0, 0, width, height);
    Image* highlightedImage = new Image(texture, width, 0, width, height);
    Image* selectedImage = new Image(texture, 2 * width, 0, width, height);
    
    Button* button = new Button(width, height, handler);
    button->setStateImage(ButtonStateNormal, normalImage);
    button->setStateImage(ButtonStateHighlighted, highlightedImage);
    button->setStateImage(ButtonStateSelected, selectedImage);
    
    normalImage->release();
    highlightedImage->release();
    selectedImage->release();
    
    texture->release();
    
    return button;
}

void gridButtonPressed(Button* button)
{
    displayView->toggleGrid();
}

void playButtonPressed(Button* button)
{
}

void pauseButtonPressed(Button* button)
{
}

void stepButtonPressed(Button* button)
{
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