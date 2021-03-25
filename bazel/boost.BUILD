cc_library(
  name = "program_options",
  srcs = [
    "lib/x86_64-linux-gnu/libboost_program_options.a"
    ],
  hdrs = glob(["include/boost/program_options/**"]) + [
    "include/boost/program_options.hpp"
  ],
  strip_include_prefix = "include",
  visibility = ["//visibility:public"],
)
