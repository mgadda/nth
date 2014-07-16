//
//  objc_helper.m
//  nth
//
//  Created by Matt Gadda on 7/13/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "test_helper.h"

#ifdef IS_XCODE
#include <CoreFoundation/CoreFoundation.h>
std::string getResourcePath() {
  CFBundleRef mainBundle = CFBundleGetMainBundle();

  CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(mainBundle);

  // Convert the URL reference into a string reference
  CFStringRef resourcePath = CFURLCopyFileSystemPath(resourceURL, kCFURLPOSIXPathStyle);

  // Get the system encoding method
  CFStringEncoding encodingMethod = CFStringGetSystemEncoding();

  // Convert the string reference into a C string
  const char *path = CFStringGetCStringPtr(resourcePath, encodingMethod);

  return std::string(path);
}
#else
std::string getResourcePath() {
  return std::string("../");
}
#endif