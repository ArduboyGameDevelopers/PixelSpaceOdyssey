//
//  DisplayView.h
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#import <Cocoa/Cocoa.h>

typedef uint8 PixelColor;
typedef uint8 PixelCord;

extern const PixelColor BLACK;
extern const PixelColor WHITE;

@class DisplayView;

@protocol DisplayViewKeyResponder <NSObject>

@optional
- (void)displayView:(DisplayView *)displayView keyDown:(NSEvent *)event;
- (void)displayView:(DisplayView *)displayView keyUp:(NSEvent *)event;

@end

@interface DisplayView : NSView

@property (nonatomic, assign) id<DisplayViewKeyResponder> keyResponder;
@property (nonatomic, assign) BOOL gridVisible;

- (void)clear;

- (PixelColor)getPixelColorX:(PixelCord)x Y:(PixelCord)y;
- (void)setPixelColor:(PixelColor)color X:(PixelCord)x Y:(PixelCord)y;

@end
