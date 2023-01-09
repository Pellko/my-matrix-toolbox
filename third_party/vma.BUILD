load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
  name = "all",
  srcs = glob(["**"]),
  visibility = ["//visibility:public"],
)

cmake(
  name = "vma",
  lib_name = select({
    "@bazel_tools//src/conditions:darwin": "libVulkanMemoryAllocator",
  }),
  lib_source = "@vma//:all",
  generate_args = ["-GNinja"],
  out_lib_dir = "lib",
  visibility = ["//visibility:public"],
)