load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
  name = "all",
  srcs = glob(["**"]),
  visibility = ["//visibility:public"],
)

cmake(
  name = "freetype_src",
  lib_name = select({
    "@bazel_tools//src/conditions:darwin": "libfreetype",
  }),
  lib_source = "@freetype//:all",
  generate_args = ["-GNinja"],
  out_lib_dir = "lib",
  visibility = ["//visibility:public"],
)

cc_library(
  name = "freetype",
  deps = [
    "@freetype//:freetype_src",
    "@png//:png",
    "@bzip2//:bzip2",
    "@zlib//:zlib",
  ],
  hdrs = glob(["include/**/*.h"]),
  includes = [
    "include/freetype2"
  ],
  strip_include_prefix = "include",
  visibility = ["//visibility:public"],
)