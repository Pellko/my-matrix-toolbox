package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "stdlib",
  srcs = [
    "Library.cc",
    "MatrixLibrary.cc",
  ],
  hdrs = [
    "Library.hh",
    "MatrixLibrary.hh",
  ],
  deps = [
    "//src/types:types",
    "//src/compiler:compiler",
    "//src/vm:vm",
    "//src/math:math",
  ],
)