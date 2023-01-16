package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "vm",
  srcs = [
    "CallFrame.cc",
    "RuntimeException.cc",
    "VirtualMachine.cc",
  ],
  hdrs = [
    "CallFrame.hh",
    "RuntimeException.hh",
    "VirtualMachine.hh",
  ],
  deps = [
    "//src/types:types",
    "//src/compiler:compiler",
    "//src/math:math",
    "//src/context:context",
  ],
)