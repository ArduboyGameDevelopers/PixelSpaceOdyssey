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
    
    // counting the rows of image[] by 8 steps each iteration
    for (int j = 0; j < HEIGHT; j+=8)
    {
        // walk the columns
        for (int i = 0; i < WIDTH; i++)
        {
            // find the relevant cell in image[]
            unsigned char c = buffer[(j/8)*WIDTH+i];
            // step through those 8-bits
            for (int k = 0; k < 8; k++)
            {
                // if the bit is set to 1, draw a pixel
                if ((c>>k)%2 > 0)
                {
                    [_displayView setPixelColor:WHITE X:i Y:j];
                }
            }
        }
    }
    
    [_displayView setNeedsDisplay:YES];
}

@end
