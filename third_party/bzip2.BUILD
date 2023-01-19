load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
  name = "all",
  srcs = glob(["**"]),
  visibility = ["//visibility:public"],
)

cmake(
  name = "bzip2",
  lib_name = select({
    "@bazel_tools//src/conditions:darwin": "libbz2",
  }),
  out_shared_libs = select({
    "@bazel_tools//src/conditions:darwin": [
      "libbz2.dylib",
      "libbz2.1.dylib",
    ],
  }),
  lib_source = "@bzip2//:all",
  generate_args = ["-GNinja"],
  out_lib_dir = "lib",
  visibility  =["//visibility:public"],
)