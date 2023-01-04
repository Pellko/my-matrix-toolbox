package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "graphics",
  srcs = [
    "Window.cc",
  ],
  hdrs = [
    "Window.hh",
  ],
  deps = [
    "@glfw//:glfw",
    "@bgfx//:bgfx",
  ],
  defines = [
    "BX_CONFIG_DEBUG=FALSE",
  ]
)