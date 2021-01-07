#include <gtest/gtest.h>

#include <sstream>
#include <vector>

#include "src/parse.h"

namespace {

struct Collector {
  void operator()(const std::string& str) {
    data.push_back(str);
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
