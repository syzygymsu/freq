alias(
  name = "src",
  actual = ":freq",
)

cc_binary(
  name = "freq",
  srcs = [
    "main.cpp"
  ],
)

cc_library(
  name = "libfreq",
  srcs = [
    "alphabet.cpp",
    "freq_count.cpp",
  ],
  hdrs = [
    "alphabet.h",
    "freq_count.h",
    "parse.h",
  ],
)

cc_test(
  name = "unittests",
  srcs = [
    "freq_count_test.cpp",
    "parse_test.cpp",
  ],
  deps = [
    ":libfreq",
    "@gtest//:gtest_main",
  ],
)
