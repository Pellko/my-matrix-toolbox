package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "vm",
  srcs = [
    "RuntimeException.cc",
    "VirtualMachine.cc",
  ],
  hdrs = [
    "RuntimeException.hh",
    "VirtualMachine.hh",
  ],
  deps = [
    "//src/types:types",
  ]
)