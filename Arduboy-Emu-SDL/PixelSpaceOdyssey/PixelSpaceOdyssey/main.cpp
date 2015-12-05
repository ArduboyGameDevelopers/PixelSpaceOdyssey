#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


#include "Button.h"
#include "DisplayView.h"
#include "EditorState.h"
#include "Image.h"
#include "Level.h"
#include "RectView.h"
#include "RootView.h"
#include "Texture.h"
#include "TileSet.h"
#include "TileView.h"
#include "bridge.h"
#include "game.h"
#include "platform.h"

typedef void (*ActionCallback)(void);

typedef struct _ButtonInfo {
    const char* textureName;
    ButtonClickHandler clickHandler;
    bool toggle;
} ButtonInfo;

typedef struct _KeyboardShortcut {
    ActionCallback action;
    SDL_Keycode sym;    /**< SDL virtual key code - see ::SDL_Keycode for details */
    Uint32 type;        /**< ::SDL_KEYDOWN or ::SDL_KEYUP */
    Uint16 mod;         /**< current key modifiers */
} KeyboardShortcut;

inline KeyboardShortcut KeyboardShortcutMake(SDL_Keycode sym,
                                             ActionCallback action,
                                             Uint32 type = SDL_KEYDOWN,
                                             Uint16 mod = 0)
{
    KeyboardShortcut shortcut;
    shortcut.action = action;
    shortcut.sym = sym;
    shortcut.type = type;
    shortcut.mod = mod;
    return shortcut;
}

static const char* const kLevelName = "level.pso";

static const int kTopViewHeight  = 48;
static const int kSideViewWidth     = 48;

static const int kDisplayViewWidth  = 512;
static const int kDisplayViewHeight = 256;

static const int kTilesViewWidth    = kDisplayViewWidth;
static const int kTilesViewHeight   = 5 * (32 + 2) + 2;

static const int kWindowWidth       = kDisplayViewWidth + kSideViewWidth;
static const int kWindowHeight      = kTopViewHeight + kDisplayViewHeight + kTilesViewHeight;

static RootView* rootView;
static DisplayView* displayView;

static Button* buttonGrid;
static Button* buttonPlay;
static Button* buttonPause;
static Button* buttonEdit;
static Button* buttonPan;

static const uint8_t LEFT_BUTTON  = 1 << 5;
static const uint8_t RIGHT_BUTTON = 1 << 2;
static const uint8_t UP_BUTTON    = 1 << 4;
static const uint8_t DOWN_BUTTON  = 1 << 6;
static const uint8_t A_BUTTON     = 1 << 1;
static const uint8_t B_BUTTON     = 1 << 0;
static int inputMask = 0;
static bool step = false;

static unsigned long gameMillis;

void createUI(SDL_Renderer* renderer);
void handleEvent(const SDL_Event* event);

void setEmulationPaused(bool paused);
void setEditMode(bool editMode);

static void shiftLevel(int dx, int dy);
static void shiftLevelLeft()   { shiftLevel(-1, 0); }
static void shiftLevelRight()  { shiftLevel(1, 0); }
static void shiftLevelTop()    { shiftLevel(0, -1); }
static void shiftLevelBottom() { shiftLevel(0, 1); }

static void toggleGrid();
static void togglePauseEmulation() { setEmulationPaused(buttonPlay->isSelected()); }
static void toggleEditMode() { setEditMode(!buttonEdit->isSelected()); }

static void startPan();
static void stopPan();

#ifdef __APPLE__
    #define MOD_CTRL KMOD_GUI
#else
    #define MOD_CTRL KMOD_CTRL
#endif

static KeyboardShortcut keyboardShortcuts[] = {
    KeyboardShortcutMake(SDLK_g, toggleGrid),
    KeyboardShortcutMake(SDLK_p, togglePauseEmulation),
    KeyboardShortcutMake(SDLK_e, toggleEditMode),
    KeyboardShortcutMake(SDLK_SPACE, startPan, SDL_KEYDOWN),
    KeyboardShortcutMake(SDLK_SPACE, stopPan, SDL_KEYUP),
    KeyboardShortcutMake(SDLK_LEFT, shiftLevelLeft, SDL_KEYDOWN, MOD_CTRL),
    KeyboardShortcutMake(SDLK_RIGHT, shiftLevelRight, SDL_KEYDOWN, MOD_CTRL),
    KeyboardShortcutMake(SDLK_UP, shiftLevelTop, SDL_KEYDOWN, MOD_CTRL),
    KeyboardShortcutMake(SDLK_DOWN, shiftLevelBottom, SDL_KEYDOWN, MOD_CTRL),
};
static int keyboardShortcutCount = sizeof(keyboardShortcuts) / sizeof(KeyboardShortcut);

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
    
    // Load level
    Level* level = Level::readFromFile(kLevelName);
    if (level != NULL)
    {
        tileMap.indices = level->indices();
        tileMap.rows = level->rows();
        tileMap.cols = level->cols();
    }
    else
    {
        level = new Level(tileMap.indices, tileMap.rows, tileMap.cols);
    }
    editorState.level = level;
    
    // Create UI
    createUI(renderer);
    
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
        
        SDL_SetRenderDrawColor(renderer, 228, 228, 228, 255);
        SDL_RenderClear(renderer);
        
        Uint32 ticks = SDL_GetTicks();
        Uint32 delta = ticks - lastFrameTime;
        if (delta >= frameDelay)
        {
            lastFrameTime = ticks;
            gameMillis += delta;
            
            if (buttonPlay->isSelected() || step)
            {
                updateGame();
                step = false;
            }
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
    
    // Destroy buttons
    buttonGrid->release();
    buttonPause->release();
    buttonPlay->release();
    buttonEdit->release();
    
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
    Uint32 type = event->type;
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
    
    
    for (int i = 0; i < keyboardShortcutCount; ++i)
    {
        const KeyboardShortcut& shortcut = keyboardShortcuts[i];
        if (type == shortcut.type && keysym.sym == shortcut.sym && (shortcut.mod == 0 || (keysym.mod & shortcut.mod)))
        {
            shortcut.action();
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
// Emulation

void setEmulationPaused(bool paused)
{
    buttonPlay->setSelected(!paused);
    buttonPause->setSelected(paused);
    if (!paused && buttonEdit->isSelected())
    {
        setEditMode(false);
    }
}

void setEditMode(bool editMode)
{
    buttonPlay->setSelected(!editMode);
    buttonPause->setSelected(editMode);
    buttonEdit->setSelected(editMode);
    displayView->setMode(editMode ? DisplayViewModeEdit : DisplayViewModeNormal);
}

////////////////////////////////////////////////////////////////////////////////////
// Button handlers

void gridButtonPressed(Button* button)
{
    displayView->toggleGrid();
    buttonGrid->setSelected(displayView->gridVisible());
}

void playButtonPressed(Button* button)
{
    setEmulationPaused(false);
}

void pauseButtonPressed(Button* button)
{
    setEmulationPaused(true);
}

void stepButtonPressed(Button* button)
{
    setEmulationPaused(true);
    step = true;
}

void editButtonPressed(Button* button)
{
    setEditMode(button->isSelected());
}

void newButtonPressed(Button* button)
{
}

void saveButtonPressed(Button* button)
{
    editorState.level->writeToFile(kLevelName);
}

void playerButtonPressed(Button* button)
{
}

////////////////////////////////////////////////////////////////////////////////////
// UI helpers

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

////////////////////////////////////////////////////////////////////////////////////
// UI: Top bar

void createTopBar(SDL_Renderer* renderer)
{
    // new level
    Button *buttonNew = createButton(renderer, "button_new.bmp", newButtonPressed);
    rootView->addView(buttonNew);
    buttonNew->release();
    
    // save level
    Button *buttonSave = createButton(renderer, "button_save.bmp", saveButtonPressed);
    rootView->addView(buttonSave);
    buttonSave->setPos(buttonNew->right(), 0);
    buttonSave->release();
    
    // play
    buttonPlay = createButton(renderer, "button_play.bmp", playButtonPressed);
    buttonPlay->setToggle(true);
    buttonPlay->setSelected(true);
    rootView->addView(buttonPlay);
    
    // pause
    buttonPause = createButton(renderer, "button_pause.bmp", pauseButtonPressed);
    buttonPause->setToggle(true);
    rootView->addView(buttonPause);
    
    // step
    Button* buttonStep = createButton(renderer, "button_step.bmp", stepButtonPressed);
    rootView->addView(buttonStep);
    buttonStep->release();
    
    int totalWidth = buttonPlay->width() + buttonPause->width() + buttonStep->width();
    int buttonX = (kWindowWidth - totalWidth) / 2;
    int buttonY = 0;
    buttonPlay->setPos(buttonX, buttonY); buttonX += buttonPlay->width();
    buttonPause->setPos(buttonX, buttonY); buttonX += buttonPause->width();
    buttonStep->setPos(buttonX, buttonY);
    
    // toggle grid
    buttonGrid = createButton(renderer, "button_grid.bmp", gridButtonPressed);
    buttonGrid->setToggle(true);
    rootView->addView(buttonGrid);
    
    // pan
    buttonPan = createButton(renderer, "button_pan.bmp", gridButtonPressed);
    buttonPan->setToggle(true);
    buttonPan->setIntractable(false);
    rootView->addView(buttonPan);
    
    buttonGrid->setPos(kWindowWidth - buttonSave->width(), buttonY);
    buttonPan->setPos(buttonGrid->left() - buttonPan->width(), buttonY);
}

////////////////////////////////////////////////////////////////////////////////////
// UI: Side bar

void createSideBar(SDL_Renderer* renderer)
{
    // edit
    buttonEdit = createButton(renderer, "button_edit.bmp", editButtonPressed);
    buttonEdit->setToggle(true);
    rootView->addView(buttonEdit);
    
    // alignment
    Button* buttons[] = {
        buttonEdit,
    };

    int buttonX = kWindowWidth - kSideViewWidth + (kSideViewWidth - 48) / 2;
    int buttonY = kTopViewHeight;
    int buttonCount = sizeof(buttons) / sizeof(Button*);
    
    for (int i = 0; i < buttonCount; ++i)
    {
        buttons[i]->setPos(buttonX, buttonY);
        buttonY += buttons[i]->height();
    }
}

////////////////////////////////////////////////////////////////////////////////////
// UI

void createUI(SDL_Renderer* renderer)
{
    rootView = new RootView(kWindowWidth, kWindowHeight);
    
    // top bar
    createTopBar(renderer);
    
    // side bar
    createSideBar(renderer);
    
    // display view
    displayView = new DisplayView(kDisplayViewWidth, kDisplayViewHeight);
    displayView->setPos(0, kTopViewHeight);
    rootView->addView(displayView);

    // tiles
    Texture* tileTexture = Texture::load(renderer, "tiles.bmp");
    TileSet* tileSet = new TileSet(tileTexture);
    TileView* tileView = new TileView(tileSet, kTilesViewWidth, kTilesViewHeight);
    tileView->setPos(0, displayView->bottom());
    rootView->addView(tileView);
    tileView->release();
    tileSet->release();
    tileTexture->release();
}

////////////////////////////////////////////////////////////////////////////////////
// Display view

void toggleGrid()
{
    displayView->toggleGrid();
    buttonGrid->setSelected(displayView->gridVisible());
}

void startPan()
{
    if (buttonEdit->isSelected())
    {
        displayView->setMode(DisplayViewModeDrag);
        buttonPan->setSelected(true);
    }
}

void stopPan()
{
    if (buttonEdit->isSelected())
    {
        displayView->setMode(DisplayViewModeEdit);
        buttonPan->setSelected(false);
    }
    else
    {
        displayView->setMode(DisplayViewModeNormal);
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Level

void shiftLevel(int dx, int dy)
{
    if (buttonEdit->isSelected())
    {
    }
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