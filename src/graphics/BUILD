package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "graphics",
  srcs = [
    "DescriptorAllocator.cc",
    "DescriptorBuilder.cc",
    "DescriptorLayoutCache.cc",
    "Font.cc",
    "FontRenderer.cc",
    "LineRenderer.cc",
    "Memory.cc",
    "Renderable.cc",
    "ShapeRenderer.cc",
    "Vertex.cc",
    "VkUtil.cc",
    "Window.cc",
  ],
  hdrs = [
    "DescriptorAllocator.hh",
    "DescriptorBuilder.hh",
    "DescriptorLayoutCache.hh",
    "Font.hh",
    "FontRenderer.hh",
    "LineRenderer.hh",
    "Memory.hh",
    "Renderable.hh",
    "ShapeRenderer.hh",
    "Vertex.hh",
    "VkUtil.hh",
    "Window.hh",
  ],
  deps = [
    "@glfw//:glfw",
    "@vkbootstrap//:vkbootstrap",
    "@vma//:vma",
    "@glm//:glm",
    "@freetype//:freetype",
    "//bazel:vulkan",
    "//src/context:runfiles",
  ],
  data  =[
    "//src/graphics/shaders:polygon_vert",
    "//src/graphics/shaders:polygon_frag",
    "//src/graphics/shaders:line_vert",
    "//src/graphics/shaders:line_frag",
    "//src/graphics/shaders:text_vert",
    "//src/graphics/shaders:text_frag",  
    "//assets:fonts",
  ]
)