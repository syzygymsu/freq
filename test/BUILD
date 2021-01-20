cc_test(
  name = "unittests",
  srcs = [
    "freq_count_test.cpp",
    "parse_test.cpp",
  ],
  deps = [
    "//src:libfreq",
    "@gtest//:gtest_main",
  ],
)

cc_binary(
  name = "find_hash_collisions",
  srcs = [
    "find_hash_collisions.cpp",
  ],
  deps = [
    "//src:libfreq",
    "@boost//:program_options",
  ],
)

cc_binary(
  name = "test_hash_collisions",
  srcs = [
    "test_hash_collisions.cpp",
  ],
  deps = [
    "//src:libfreq",
    "@boost//:program_options",
  ],
)
