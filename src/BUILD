alias(
  name = "src",
  actual = ":freq",
)

cc_binary(
  name = "freq",
  srcs = [
    "main.cpp"
  ],
  deps = [
    ":libfreq",
  ],
)

cc_library(
  name = "libfreq",
  srcs = [
    "alphabet.cpp",
    "cli.cpp",
    "freq_count.cpp",
  ],
  hdrs = [
    "alphabet.h",
    "cli.h",
    "freq_count.h",
    "parse.h",
  ],
  deps = [
    "@boost//:program_options",
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
