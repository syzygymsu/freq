#include <gtest/gtest.h>

#include "freq_count.h"

TEST(FreqCount, Sample) {
  const std::string input =
      "The time has come, the Walrus said,\n"
      "to talk of many things...";
  const FreqCountResult expected{
      {"the", 2},
      {"come", 1},
      {"has", 1},
      {"many", 1},
      {"of", 1},
      {"said", 1},
      {"talk", 1},
      {"things", 1},
      {"time", 1},
      {"to", 1},
      {"walrus", 1},
  };
  std::istringstream input_stream(input);

  EXPECT_EQ(expected, FreqCount(input_stream));
}
