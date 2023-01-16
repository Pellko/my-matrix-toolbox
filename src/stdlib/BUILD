package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "stdlib",
  srcs = [
    "Library.cc",
    "MatrixLibrary.cc",
    "PlotLibrary.cc",
  ],
  hdrs = [
    "Library.hh",
    "MatrixLibrary.hh",
    "PlotLibrary.hh",
  ],
  deps = [
    "//src/types:types",
    "//src/compiler:compiler",
    "//src/vm:vm",
    "//src/graphics:graphics",
    "//src/math:math",
    "//src/context:context",
  ],
)