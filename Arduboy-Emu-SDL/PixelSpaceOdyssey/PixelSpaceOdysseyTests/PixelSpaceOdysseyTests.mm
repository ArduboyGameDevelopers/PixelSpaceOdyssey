//
//  PixelSpaceOdysseyTests.m
//  PixelSpaceOdysseyTests
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "List.h"

typedef List<int> TestList;

@interface PixelSpaceOdysseyTests : XCTestCase

@end

@implementation PixelSpaceOdysseyTests

- (void)testAdd
{
    TestList list;
    list.add(0);
    list.add(1);
    list.add(2);

    [self assertList:&list, @0, @1, @2, nil];
}

- (void)testRemove
{
    TestList list;
    list.add(0);
    list.add(1);
    list.add(2);
    
    list.remove(1);
    [self assertList:&list, @0, @2, nil];
    
    list.remove(1);
    [self assertList:&list, @0, @2, nil];
    
    list.remove(0);
    [self assertList:&list, @2, nil];
    
    list.remove(2);
    [self assertList:&list, nil];
}

#pragma mark -
#pragma mark Helpers

- (void)assertList:(const TestList*)list, ... NS_REQUIRES_NIL_TERMINATION
{
    NSMutableArray *expected = [NSMutableArray array];
    
    va_list ap;
    va_start(ap, list);
    for (id number = va_arg(ap, id); number != nil; number = va_arg(ap, id))
    {
        [expected addObject:number];
    }
    va_end(ap);
    
    XCTAssertEqual(expected.count, list->size());
    for (int i = 0; i < list->size(); ++i)
    {
        XCTAssertEqual([[expected objectAtIndex:i] intValue], list->get(i));
    }

}

@end
