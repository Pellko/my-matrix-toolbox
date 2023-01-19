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

# VkBoostrap Library
new_git_repository(
  name = "vkbootstrap",
  remote = "https://github.com/charles-lunarg/vk-bootstrap.git",
  commit = "b4b177170e784c2aa99a31229d78aa5fe489ad7a",
  build_file = "@//third_party:vkbootstrap.BUILD",
)

# VMA Library
new_git_repository(
  name = "vma",
  remote = "https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git",
  commit = "8b87b6cbf765d2b4d6cf66a06cc9004e11c096c4",
  build_file = "@//third_party:vma.BUILD",
)

# GLM Library
new_git_repository(
  name = "glm",
  remote = "https://github.com/g-truc/glm.git",
  commit = "fc8f4bb442b9540969f2f3f351c4960d91bca17a",
  build_file = "@//third_party:glm.BUILD",
)

# FreeType Library
new_git_repository(
  name = "freetype",
  remote = "https://gitlab.freedesktop.org/freetype/freetype.git",
  commit = "e8ebfe988b5f57bfb9a3ecb13c70d9791bce9ecf",
  build_file = "@//third_party:freetype.BUILD",
)

# PNG Library
new_git_repository(
  name = "png",
  remote = "https://github.com/glennrp/libpng.git",
  commit = "07b8803110da160b158ebfef872627da6c85cbdf",
  build_file = "@//third_party:png.BUILD",
)

# BZip2 Library
new_git_repository(
  name = "bzip2",
  remote = "https://gitlab.com/bzip2/bzip2.git",
  commit = "2d8393924b9f3e014000c7420c7da7c3ddb74e2c",
  build_file = "@//third_party:bzip2.BUILD",
)

# ZLib Library
new_git_repository(
  name = "zlib",
  remote = "https://github.com/madler/zlib.git",
  commit = "04f42ceca40f73e2978b50e93806c2a18c1281fc",
  build_file = "@//third_party:zlib.BUILD",
)

# Compilation database tooling
http_archive(
  name = "hedron_compile_commands",
  url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/5ceebcf367f51c6a18fffe726755951a43b50e45.tar.gz",
  strip_prefix = "bazel-compile-commands-extractor-5ceebcf367f51c6a18fffe726755951a43b50e45",
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()

# Vulkan dependency
load("//bazel:vulkan.bzl", "vulkan_repositories")
vulkan_repositories()

# Rules foreign cc
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
  name = "rules_foreign_cc",
  strip_prefix = "rules_foreign_cc-f1ac5b47265d964ee670f0ff2ff4552c7026f600",
  url = "https://github.com/bazelbuild/rules_foreign_cc/archive/f1ac5b47265d964ee670f0ff2ff4552c7026f600.tar.gz",
)
load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()