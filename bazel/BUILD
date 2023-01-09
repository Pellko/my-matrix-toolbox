load("@rules_cc//cc:defs.bzl", "cc_library")
load("//bazel:vulkan.bzl", "glsl_toolchain")

# Vulkan libraries
VULKAN_LIBRARIES = select({
  "@bazel_tools//src/conditions:darwin": ["@vulkan_macos//:vulkan"],
  "@bazel_tools//src/conditions:windows": [],
  "@bazel_tools//src/conditions:linux_x86_64": [],
})

cc_library(
  name = "vulkan",
  deps = VULKAN_LIBRARIES,
  visibility = ["//visibility:public"],
)

# Shader compiler toolchain
toolchain_type(
  name = "glsl_toolchain_type",
)

glsl_toolchain(
  name = "on_macos",
  glslc = "@vulkan_macos//:glslc"
)

toolchain(
  name = "glsl_macos_toolchain",
  exec_compatible_with = [
    "@platforms//os:macos",
  ],
  toolchain = ":on_macos",
  toolchain_type = "//bazel:glsl_toolchain_type"
)