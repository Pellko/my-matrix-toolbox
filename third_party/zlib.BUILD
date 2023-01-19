load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
  name = "all",
  srcs = glob(["**"]),
  visibility = ["//visibility:public"],
)

cmake(
  name = "zlib",
  lib_name = select({
    "@bazel_tools//src/conditions:darwin": "libz",
  }),
  lib_source = "@zlib//:all",
  generate_args = ["-GNinja"],
  out_lib_dir = "lib",
  visibility  =["//visibility:public"],
)