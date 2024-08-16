#include "common.h"
#include "gtest/gtest.h"
#include "parser.h"
#include "pretty.h"

namespace tmonkey {
namespace {

TEST(LexerTests, Basics) {
  StringInterningMap str_map;
  {
    auto idx = str_map.intern("q");
    auto res = str_map.string(idx);
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ("q", *res);
  }

  {
    auto idx = str_map.intern("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq");
    auto res = str_map.string(idx);
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq", *res);
  }
}

}  // namespace
}  // namespace tmonkey