//
//  NSEvent+Keyboard.h
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSEvent (Keyboard)

- (unichar)keyChar;

@end
