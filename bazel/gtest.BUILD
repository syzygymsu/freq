cc_library(
  name = "gtest",
  srcs = [
    "lib/libgtest.a"
  ],
  hdrs = glob(["include/gtest/**"]),
  deps = [
    "@pthread",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "gtest_main",
  srcs = [
    "lib/libgtest_main.a",
  ],
  deps = [
    ":gtest",
  ],
  visibility = ["//visibility:public"],
)
