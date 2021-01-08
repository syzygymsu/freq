#include <gtest/gtest.h>

#include <sstream>
#include <vector>

#include "src/parse.h"

namespace {

std::string RestoreWord(std::string_view s) {
  std::string res;
  res.reserve(s.size());
  std::transform(
      s.begin(),
      s.end(),
      std::back_inserter(res),
      &RestoreChar);
  return res;
}

struct Collector {
  void operator()(const std::string_view str) {
    data.push_back(std::string(str));
  }

  std::vector<std::string> Restore() const {
    std::vector<std::string> res(data.size());
    std::transform(
        data.begin(),
        data.end(),
        res.begin(),
        &::RestoreWord);
    return res;
  }

  std::vector<std::string> data;
};

}

TEST(Parse, Simple) {
  const std::string input = "FOO bar123ZeE";
  const std::vector<std::string> expected = {"foo", "bar", "zee"};
  std::istringstream input_stream(input);

  Collector collector;
  Parse(input_stream, collector);

  EXPECT_EQ(expected, collector.Restore());
}

TEST(Parse, FullCharRange) {
  std::string input;
  for (char c = char{-128}; ; c++) {
    input.push_back(c);
    if (char{127} == c) {
      break;
    }
  }

  const std::vector<std::string> expected = {"abcdefghijklmnopqrstuvwxyz", "abcdefghijklmnopqrstuvwxyz"};
  std::istringstream input_stream(input);

  Collector collector;
  Parse(input_stream, collector);

  EXPECT_EQ(expected, collector.Restore());
}
