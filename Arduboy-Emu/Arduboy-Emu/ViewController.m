//
//  ViewController.m
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#import "Platform/platform.h"
#import "ViewController.h"
#import "DisplayView.h"
#import "NSEvent+Keyboard.h"

#import "game.h"

static const uint8_t LEFT_BUTTON  = 1 << 5;
static const uint8_t RIGHT_BUTTON = 1 << 2;
static const uint8_t UP_BUTTON    = 1 << 4;
static const uint8_t DOWN_BUTTON  = 1 << 6;
static const uint8_t A_BUTTON     = 1 << 1;
static const uint8_t B_BUTTON     = 1 << 0;

static ViewController * _instance;

@interface ViewController () <DisplayViewKeyResponder>

@property (nonatomic, assign) IBOutlet DisplayView *displayView;
@property (atomic, assign) uint8_t inputMask;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _instance = self;
    
    [self.view.window makeFirstResponder:_displayView];
    _displayView.keyResponder = self;

    NSThread *thread = [[NSThread alloc] initWithTarget:self selector:@selector(runGame) object:nil];
    [thread start];
}

- (void)runGame
{
    startGame();
    
    NSTimeInterval frameDelay = 1.0/60;
    
    while (true)
    {
        NSTimeInterval time = CFAbsoluteTimeGetCurrent();
        
        // loop
        loopGame();
        
        NSTimeInterval frameTime = CFAbsoluteTimeGetCurrent() - time;
        NSTimeInterval sleepTime = frameDelay - frameTime;
        if (sleepTime > 0)
        {
            [NSThread sleepForTimeInterval:sleepTime];
        }
    }
}

- (void)renderScreenBuffer:(const unsigned char *)screenBuffer width:(int)width height:(int)height
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            uint8_t row = (uint8_t)y / 8;
            int index = row * width + (uint8_t)x;
            int bit = screenBuffer[index] & (1 << ((uint8_t)y % 8));
            PixelColor color = bit ? WHITE : BLACK;
            [_displayView setPixelColor:color X:x Y:y];
        }
    }
    
    [_displayView setNeedsDisplay:YES];
}

#pragma mark -
#pragma mark DisplayViewKeyResponder

- (void)displayView:(DisplayView *)displayView keyDown:(NSEvent *)event
{
    uint8_t mask = [self buttonMaskForKey:event.keyChar];
    if (mask != 0)
    {
        self.inputMask |= mask;
    }
}

- (void)displayView:(DisplayView *)displayView keyUp:(NSEvent *)event
{
    uint8_t mask = [self buttonMaskForKey:event.keyChar];
    if (mask != 0)
    {
        self.inputMask &= ~mask;
    }
}

- (uint8_t)buttonMaskForKey:(unichar)key
{
    switch (key)
    {
        case 'w': return UP_BUTTON;
        case 's': return DOWN_BUTTON;
        case 'a': return LEFT_BUTTON;
        case 'd': return RIGHT_BUTTON;
        case 'n': return B_BUTTON;
        case 'm': return A_BUTTON;
    }
    
    return 0;
}

#pragma mark -
#pragma mark Platform

void platformRenderScreen(unsigned const char* screenBuffer, int width, int height)
{
    // render is sync to avoid nasty surprises
    dispatch_sync(dispatch_get_main_queue(), ^{
        [_instance renderScreenBuffer:screenBuffer width:width height:height];
    });
}

unsigned long platformMillis(void)
{
    return (unsigned long) (CFAbsoluteTimeGetCurrent() * 1000);
}

void platformDelay(unsigned long millis)
{
    [NSThread sleepForTimeInterval:0.001 * millis];
}

uint8_t platformInput()
{
    return [_instance inputMask];
}

@end