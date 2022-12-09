load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")

def custom_cc_library(name, copts=[], linkopts=[], **kwargs):
  cc_library(
    name = name,
    copts = copts + global_copts(),
    linkopts = linkopts + global_linkopts(),
    **kwargs
  )

def custom_cc_binary(name, copts=[], linkopts=[], **kwargs):
  cc_binary(
    name = name,
    copts = copts + global_copts(),
    linkopts = linkopts + global_linkopts(),
    **kwargs
  )

def global_copts():
  return select({
    "@bazel_tools//src/conditions:windows": [
      "/std:c++20",
      "/MT",
    ],
    "@bazel_tools//src/conditions:darwin": [
      "-std=c++20"
    ]
  })

def global_linkopts():
  return select({
    "@bazel_tools//src/conditions:darwin": []
  })