//
//  NSEvent+Keyboard.m
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#import "NSEvent+Keyboard.h"

@implementation NSEvent (Keyboard)

- (unichar)keyChar
{
    NSString *chars = self.characters;
    return chars.length > 0 ? [chars characterAtIndex:0] : 0;
}

@end
