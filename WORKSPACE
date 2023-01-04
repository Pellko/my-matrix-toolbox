load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

# GLFW Library
GLFW_VERSION = "3.3.5"
http_archive(
  name = "glfw",
  build_file = "@//third_party:glfw.BUILD",
  sha256 = "a89bb6074bc12bc12fcd322dcf848af81b679ccdc695f70b29ca8a9aa066684b",
  strip_prefix = "glfw-{}".format(GLFW_VERSION),
  urls = ["https://github.com/glfw/glfw/archive/{}.zip".format(GLFW_VERSION)],
)

# BGFX Library
new_git_repository(
  name = "bgfx",
  commit = "ac2c644f44af2ab608a3ea3afa3bdc034d6e8980",
  remote = "https://github.com/bkaradzic/bgfx.cmake.git",
  build_file = "@//third_party:bgfx.BUILD",
  recursive_init_submodules	= True,
)

# Compilation database tooling
http_archive(
  name = "hedron_compile_commands",
  url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/5ceebcf367f51c6a18fffe726755951a43b50e45.tar.gz",
  strip_prefix = "bazel-compile-commands-extractor-5ceebcf367f51c6a18fffe726755951a43b50e45",
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()

# Rules foreign CC
http_archive(
  name = "rules_foreign_cc",
  sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
  strip_prefix = "rules_foreign_cc-0.9.0",
  url = "https://github.com/bazelbuild/rules_foreign_cc/archive/refs/tags/0.9.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()