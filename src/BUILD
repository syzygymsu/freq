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
  ],
  hdrs = [
    "alphabet.h",
    "parse.h",
  ],
)

cc_test(
  name = "parse_test",
  srcs = ["parse_test.cpp"],
  deps = [
    ":libfreq",
    "@gtest//:gtest_main",
  ],
)
