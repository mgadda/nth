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
@property std::string resource_path;
@end

@implementation DriverTest

- (void)setUp
{
    [super setUp];
    self.resource_path = std::string([[[NSBundle bundleForClass:[self class]] resourcePath] UTF8String]);
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testParse {
  nth::Driver d;
//  std::string resource_path = std::string(
//    [[[NSBundle bundleForClass:[self class]] resourcePath] UTF8String]);
  
  XCTAssertEqual(0, d.parse(self.resource_path + "/nth.nth"), @"Parsing nth.nth did not return 0 exit status.");
}

- (void)testCanParseInt {
  nth::Driver d;
  std::string str = "10";
  int res = d.parseString(str);
  
  XCTAssertEqual(0, res, "Could not parse: %s", str.c_str());
  XCTAssertNotEqual(nullptr, d.result, "Expected d.result to be non-null");
  nth::Block *fileBlock = d.result;

  XCTAssertNotEqual(false, fileBlock->getExpressions().empty(), "Expressions should not be empty");  
}

@end
