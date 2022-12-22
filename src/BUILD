load("//bazel:custom_library.bzl", "custom_cc_binary")

custom_cc_binary(
  name = "mymatrixtoolbox",
  srcs = [
    "main.cc"
  ],
  deps = [
    "//src/compiler:compiler",
    "//src/vm:vm",
  ],
  data = [
    "//examples:examples",
  ]
)