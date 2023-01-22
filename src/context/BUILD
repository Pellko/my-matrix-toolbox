package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "context",
  srcs = [
    "ExecutionContext.cc",
  ],
  hdrs = [
    "ExecutionContext.hh",
  ],
  deps = [
    "//src/graphics:graphics",
    "//src/types:types",
  ],
)

custom_cc_library(
  name = "runfiles",
  srcs = [
    "Runfiles.cc",
  ],
  hdrs = [
    "Runfiles.hh"
  ],
  deps = [
    "@wai//:wai",
  ]
)