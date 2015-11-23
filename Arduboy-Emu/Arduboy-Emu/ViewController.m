//
//  ViewController.m
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#import "ViewController.h"
#import "game.h"

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    startGame();
    
    [NSTimer timerWithTimeInterval:1.0/60
                            target:self
                          selector:@selector(gameLoop)
                          userInfo:nil
                           repeats:YES];
}


- (void)gameLoop
{
    loopGame();
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

@end
