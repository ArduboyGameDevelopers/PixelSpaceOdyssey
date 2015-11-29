//
//  DisplayView.m
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#import "DisplayView.h"
#import "game.h"
#import "drawing.h"

static const int DISPLAY_WIDTH  = 128;
static const int DISPLAY_HEIGHT = 64;
static const int GRID_SIZE      = 8;

#define DISPLAY_BUFFER_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(PixelColor))

const PixelColor BLACK = 0;
const PixelColor WHITE = 1;

#define BLOCK_COLOR_WHITE 0.97, 0.99, 1.0, 1.0
#define BLOCK_COLOR_BLACK 0, 0, 0, 1
#define BLOCK_COLOR_MAGENTA 1, 0, 1, 1
#define GRID_COLOR 0.945, 0.96, 0.972, 1.0

static inline int getPixelIndex(int x, int y)
{
    assert(x >= 0 && x < DISPLAY_WIDTH);
    assert(y >= 0 && y < DISPLAY_HEIGHT);
    
    return y * DISPLAY_WIDTH + x;
}

@interface DisplayView ()
{
    CGFloat _pixelWidth;
    CGFloat _pixelHeight;
    PixelColor * _displayBuffer;
    
    BOOL _hasMouse;
    int _mouseCellX;
    int _mouseCellY;
}
@end

@implementation DisplayView

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self)
    {
        NSRect bounds = self.bounds;
        _pixelWidth = bounds.size.width / DISPLAY_WIDTH;
        _pixelHeight = bounds.size.height / DISPLAY_HEIGHT;

        NSTrackingAreaOptions trackingOptions =
            NSTrackingActiveAlways |
            NSTrackingMouseEnteredAndExited |
            NSTrackingMouseMoved;
        
        NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:bounds
                                                                    options:trackingOptions
                                                                      owner:self
                                                                   userInfo:nil];
        [self addTrackingArea:trackingArea];
        
        _displayBuffer = (PixelColor *) malloc(DISPLAY_BUFFER_SIZE);
        [self clear];
    }
    return self;
}

#pragma mark -
#pragma mark Mouse

- (void)mouseEntered:(NSEvent *)theEvent
{
    _hasMouse = YES;
}

- (void)mouseExited:(NSEvent *)theEvent
{
    _hasMouse = NO;
}

- (void)mouseMoved:(NSEvent *)theEvent
{
}

#pragma mark -
#pragma mark Drawing

- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef context = (CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
    CGContextSetRGBFillColor(context, BLOCK_COLOR_WHITE);
    CGContextFillRect(context, self.bounds);
    
    CGContextTranslateCTM(context, 0, CGRectGetHeight(self.bounds));
    CGContextScaleCTM(context, 1, -1);
    
    if (_gridVisible)
    {
        int gridRows = tileMapHeight / GRID_SIZE;
        int gridCols = tileMapWidth / GRID_SIZE;
        CGFloat cellWidth = [self toPixelsHor:GRID_SIZE];
        CGFloat cellHeight = [self toPixelsVer:GRID_SIZE];;
        
        CGFloat transX = [self toPixelsHor:drawTransX];
        CGFloat transY = [self toPixelsHor:drawTransY];
        
        CGContextTranslateCTM(context, transX, transY);
        
        CGContextSetRGBFillColor(context, GRID_COLOR);
        
        for (int row = 0; row < gridRows; ++row)
        {
            BOOL visible = row % 2 == 0;
            for (int col = 0; col < gridCols; ++col)
            {
                if (visible)
                {
                    CGRect rect = CGRectMake(col * cellWidth, row * cellWidth, cellWidth, cellHeight);
                    CGContextFillRect(context, rect);
                }
                
                visible = !visible;
            }
        }
        
        CGContextTranslateCTM(context, -transX, -transY);
    }
    
    for (int y = 0; y < DISPLAY_HEIGHT; ++y)
    {
        for (int x = 0; x < DISPLAY_WIDTH; ++x)
        {
            PixelColor color = [self getPixelColorX:x Y:y];
            if (color == WHITE)
            {
                continue;
            }
            
            if (color == BLACK)
            {
                CGContextSetRGBFillColor(context, BLOCK_COLOR_BLACK);
            }
            else
            {
                CGContextSetRGBFillColor(context, BLOCK_COLOR_MAGENTA);
            }
            
            CGRect rect = CGRectMake(x * _pixelWidth, y * _pixelHeight, _pixelWidth, _pixelHeight);
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
#pragma mark Helpers

- (CGFloat)toPixelsHor:(int16_t)value
{
    return value * _pixelWidth;
}

- (CGFloat)toPixelsVer:(int16_t)value
{
    return value * _pixelHeight;
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
