package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "graphics",
  srcs = [
    "VkMemory.cc",
    "VkMesh.cc",
    "VkUtil.cc",
    "Window.cc",
  ],
  hdrs = [
    "VkMemory.hh",
    "VkMesh.hh",
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
    "//src/graphics/shaders:triangle_vert",
    "//src/graphics/shaders:triangle_frag",
  ]
)