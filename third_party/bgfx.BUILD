load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
  name = "all",
  srcs = glob(["**"]),
  visibility = ["//visibility:public"],
)

cmake(
  name = "bgfx",
  out_static_libs = select({
    "@bazel_tools//src/conditions:darwin": [
      "libbgfx.a",
      "libbimg.a",
      "libbx.a",
      "libbgfx-tinyexr.a",
      "libbgfx-squish.a",
      "libbgfx-pvrtc.a",
      "libbgfx-nvtt.a",
      "libbgfx-iqa.a",
      "libbgfx-etc2.a",
      "libbgfx-etc1.a",
      "libbgfx-edtaa3.a",
      "libastc-encoder.a",
    ]
  }),
  lib_source = "@bgfx//:all",
  generate_args = ["-GNinja"],
  out_lib_dir = "lib",
  # copts = select({
  #   "@bazel_tools//src/conditions:darwin": [
  #     "-fobjc-arc",
  #   ]
  # }),
  # linkopts = select({
  #   "@bazel_tools//src/conditions:darwin": [
  #     "-framework Metal",
  #   ],
  # }),
  visibility = ["//visibility:public"],
)