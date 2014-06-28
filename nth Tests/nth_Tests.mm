//
//  nth_Tests.m
//  nth Tests
//
//  Created by Matt Gadda on 6/27/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "Driver.h"

@interface nth_Tests : XCTestCase

@end

@implementation nth_Tests

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample
{
  XCTAssertEqual(0, 0, @"0 was 0");
    //XCTFail(@"No implementation for \"%s\"", __PRETTY_FUNCTION__);
}

//- (void)testParse {
//  nth::Driver *d = new nth::Driver();
//  d->parse("somefile.nth");
//}

@end
