load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Compilation database tooling
http_archive(
  name = "hedron_compile_commands",
  url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/5ceebcf367f51c6a18fffe726755951a43b50e45.tar.gz",
  strip_prefix = "bazel-compile-commands-extractor-5ceebcf367f51c6a18fffe726755951a43b50e45",
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()