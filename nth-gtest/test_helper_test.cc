#include <gtest/gtest.h>
#include "test_helper.h"

class TestHelperTest : public ::testing::Test {
protected:
  virtual void SetUp() {}
};

//TEST_F(TestHelperTest, StripMargin) {
//  std::string stripped = stripMargin("a\n\
//                                     | b\n\
//                                     | c");
//  
//  EXPECT_STREQ("a\nb\nc", stripped.c_str());
//}
