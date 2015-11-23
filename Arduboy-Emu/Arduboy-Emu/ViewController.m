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

#import "game.h"

static ViewController * _instance;

@interface ViewController ()

@property (nonatomic, assign) IBOutlet DisplayView *displayView;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _instance = self;

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

@end