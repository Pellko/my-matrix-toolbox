load("//bazel:custom_library.bzl", "custom_cc_binary")

custom_cc_binary(
  name = "mymatrixtoolbox",
  srcs = [
    "main.cc"
  ],
  deps = [
    "//src/compiler:compiler",
    "//src/vm:vm",
    "//src/math:math",
    "//src/stdlib:stdlib",
    "//src/graphics:graphics",
    "//src/context:context",
  ],
  data = [
    "//examples:examples",
  ]
)