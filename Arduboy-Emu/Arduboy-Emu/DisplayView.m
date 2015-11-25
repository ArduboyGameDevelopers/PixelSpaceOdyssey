//
//  DisplayView.m
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#import "DisplayView.h"

static const int DISPLAY_WIDTH  = 128.0;
static const int DISPLAY_HEIGHT = 64.0;

#define DISPLAY_BUFFER_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(PixelColor))

const PixelColor BLACK = 0;
const PixelColor WHITE = 1;

#define BLOCK_COLOR_WHITE 0.97, 0.99, 1.0, 1.0
#define BLOCK_COLOR_BLACK 0, 0, 0, 1
#define BLOCK_COLOR_MAGENTA 1, 0, 1, 1

static inline int getPixelIndex(int x, int y)
{
    assert(x >= 0 && x < DISPLAY_WIDTH);
    assert(y >= 0 && y < DISPLAY_HEIGHT);
    
    return y * DISPLAY_WIDTH + x;
}

@interface DisplayView ()
{
    CGFloat _blockWidth;
    CGFloat _blockHeight;
    PixelColor * _displayBuffer;
}
@end

@implementation DisplayView

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self)
    {
        NSRect bounds = self.bounds;
        _blockWidth = bounds.size.width / DISPLAY_WIDTH;
        _blockHeight = bounds.size.height / DISPLAY_HEIGHT;
        
        _displayBuffer = (PixelColor *) malloc(DISPLAY_BUFFER_SIZE);
        [self clear];
    }
    return self;
}

#pragma mark -
#pragma mark Drawing

- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef context = (CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
    
    for (int y = 0; y < DISPLAY_HEIGHT; ++y)
    {
        for (int x = 0; x < DISPLAY_WIDTH; ++x)
        {
            PixelColor color = [self getPixelColorX:x Y:y];
            if (color == WHITE)
            {
                CGContextSetRGBFillColor(context, BLOCK_COLOR_WHITE);
            }
            else if (color == BLACK)
            {
                CGContextSetRGBFillColor(context, BLOCK_COLOR_BLACK);
            }
            else
            {
                CGContextSetRGBFillColor(context, BLOCK_COLOR_MAGENTA);
            }
            
            CGRect rect = CGRectMake(x * _blockWidth, (DISPLAY_HEIGHT - 1 - y) * _blockHeight, _blockWidth, _blockHeight);
            CGContextFillRect(context, rect);
        }
    }
}

#pragma mark -
#pragma mark Event handling

- (void)keyDown:(NSEvent *)theEvent
{
    if ([_keyResponder respondsToSelector:@selector(displayView:keyDown:)])
    {
        [_keyResponder displayView:self keyDown:theEvent];
    }
}

- (void)keyUp:(NSEvent *)theEvent
{
    if ([_keyResponder respondsToSelector:@selector(displayView:keyUp:)])
    {
        [_keyResponder displayView:self keyUp:theEvent];
    }
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

#pragma mark -
#pragma mark Display

- (void)clear
{
    memset(_displayBuffer, BLACK, DISPLAY_BUFFER_SIZE);
}

- (PixelColor)getPixelColorX:(PixelCord)x Y:(PixelCord)y
{
    int index = getPixelIndex(x, y);
    return _displayBuffer[index];
}

- (void)setPixelColor:(PixelColor)color X:(PixelCord)x Y:(PixelCord)y
{
    int index = getPixelIndex(x, y);
    _displayBuffer[index] = color;
}

@end
