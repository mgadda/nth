//
//  DriverTest.m
//  nth
//
//  Created by Matt Gadda on 6/28/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "driver.h"

@interface DriverTest : XCTestCase

@end

@implementation DriverTest

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

- (void)testMainParse {
  std::string resource_path = std::string(
    [[[NSBundle bundleForClass:[self class]] resourcePath] UTF8String]);
  
  
  std::string nth_filename = resource_path + "/nth.nth";
  
  XCTAssertEqual(0, nth::parse(nth_filename), @"Parsing nth.nth did not return 0.");
}

- (void)testParse
{
  nth::Driver *d = new nth::Driver();
  XCTAssertEqual(0, d->parse("nth.nth"), @"Parsing nth.nth did not return 0 exit status.");
}

@end
