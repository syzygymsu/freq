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
    "freq_counter.cpp",
  ],
  hdrs = [
    "alphabet.h",
    "cli.h",
    "freq_counter.h",
    "parse.h",
  ],
  deps = [
    "@boost//:program_options",
  ],
  visibility = ["//visibility:public"],
)
