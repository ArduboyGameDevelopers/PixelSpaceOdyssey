//
//  ViewController.m
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#import "ViewController.h"
#import "DisplayView.h"

#import "game.h"

@interface ViewController ()

@property (nonatomic, assign) IBOutlet DisplayView *displayView;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

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
        
        // render screen
        int bufferLength;
        unsigned char* buffer = getDisplayBuffer(&bufferLength);
        dispatch_sync(dispatch_get_main_queue(), ^{
            [self renderBuffer:buffer length:bufferLength];
        });
        
        NSTimeInterval frameTime = CFAbsoluteTimeGetCurrent() - time;
        NSTimeInterval sleepTime = frameDelay - frameTime;
        if (sleepTime > 0)
        {
            [NSThread sleepForTimeInterval:sleepTime];
        }
    }
}

- (void)renderBuffer:(const unsigned char *)buffer length:(int)length
{
    int WIDTH = getDisplayWidth();
    int HEIGHT = getDisplayHeight();
    
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            uint8_t row = (uint8_t)y / 8;
            int index = row * WIDTH + (uint8_t)x;
            int bit = buffer[index] & (1 << ((uint8_t)y % 8));
            PixelColor color = bit ? WHITE : BLACK;
            [_displayView setPixelColor:color X:x Y:y];
        }
    }
    
    [_displayView setNeedsDisplay:YES];
}

@end
