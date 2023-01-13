package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "graphics",
  srcs = [
    "Memory.cc",
    "Polygon.cc",
    "ShapeRenderer.cc",
    "VkUtil.cc",
    "Window.cc",
  ],
  hdrs = [
    "Memory.hh",
    "Polygon.hh",
    "ShapeRenderer.hh",
    "VkUtil.hh",
    "Window.hh",
  ],
  deps = [
    "@glfw//:glfw",
    "@vkbootstrap//:vkbootstrap",
    "@vma//:vma",
    "@glm//:glm",
    "//bazel:vulkan",
  ],
  data  =[
    "//src/graphics/shaders:polygon_vert",
    "//src/graphics/shaders:polygon_frag",
  ]
)