//
//  Tests.m
//  Tests
//
//  Created by Alex Lementuev on 12/19/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "dispatch.h"

static NSMutableArray *result;

static void dispatchCallback(void *user, int16_t data1, int16_t data2)
{
    [result addObject:[NSNumber numberWithInt:data1]];
}

@interface DispatchTest : XCTestCase

@end

@implementation DispatchTest

- (void)setUp
{
    [super setUp];
    result = [NSMutableArray new];
}


- (void)tearDown
{
    DispatchShutdown();
    [super tearDown];
}

- (void)test1
{
    DispatchUpdate(50);
    [self assertResult:@[]];
    
    // 2 -> 3 -> 1
    DispatchSchedule(dispatchCallback, 200, NULL, 1, 0);
    DispatchSchedule(dispatchCallback, 150, NULL, 3, 0);
    DispatchSchedule(dispatchCallback, 100, NULL, 2, 0);
    
    DispatchUpdate(50);                 // 50
    [self assertResult:@[]];
    
    DispatchUpdate(50);                 // 100
    [self assertResult:@[ @2 ]];
    
    DispatchUpdate(50);                 // 150
    [self assertResult:@[ @3 ]];
    
    DispatchUpdate(50);                 // 200
    [self assertResult:@[ @1 ]];
    
    DispatchUpdate(50);                 // 250
    [self assertResult:@[]];
}

- (void)test2
{
    DispatchUpdate(50);
    [self assertResult:@[]];
    
    // 2 -> 3 -> 1
    DispatchSchedule(dispatchCallback, 200, NULL, 1, 0);
    DispatchSchedule(dispatchCallback, 150, NULL, 3, 0);
    DispatchSchedule(dispatchCallback, 100, NULL, 2, 0);
    
    DispatchUpdate(50);                 // 50
    [self assertResult:@[]];
    
    DispatchUpdate(50);                 // 100
    [self assertResult:@[ @2 ]];
    
    DispatchUpdate(50);                 // 150
    [self assertResult:@[ @3 ]];
    
    DispatchUpdate(50);                 // 200
    [self assertResult:@[ @1 ]];
    
    DispatchUpdate(50);                 // 250
    [self assertResult:@[]];
}

- (void)test3
{
    DispatchUpdate(50);
    [self assertResult:@[]];
    
    // 3 -> 2 -> 1

    DispatchSchedule(dispatchCallback, 200, NULL, 1, 0);
    DispatchSchedule(dispatchCallback, 100, NULL, 2, 0);
    DispatchSchedule(dispatchCallback, 150, NULL, 3, 0);
    
    DispatchUpdate(50);                 // 50
    [self assertResult:@[]];
    
    DispatchUpdate(50);                 // 100
    [self assertResult:@[ @2 ]];
    
    DispatchUpdate(50);                 // 150
    [self assertResult:@[ @3 ]];
    
    DispatchUpdate(50);                 // 200
    [self assertResult:@[ @1 ]];
    
    DispatchUpdate(50);                 // 250
    [self assertResult:@[]];
}

#pragma mark -
#pragma mark Helpers

- (void)assertResult:(NSArray *)expected
{
    XCTAssertEqual(expected.count, result.count);
    for (int i = 0; i < expected.count; ++i)
    {
        XCTAssertEqual(expected[i], result[i]);
    }
    
    [result removeAllObjects];
}

@end
