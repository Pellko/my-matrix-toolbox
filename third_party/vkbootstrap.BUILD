load("@//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "vkbootstrap",
  srcs = [
    "src/VkBootstrap.cpp"
  ],
  hdrs = [
    "src/VkBootstrap.h",
    "src/VkBootstrapDispatch.h",
  ],
  deps = [
    "@//bazel:vulkan"
  ],
  include_prefix = "vkb",
  strip_include_prefix = "src",
  visibility = ["//visibility:public"],
)