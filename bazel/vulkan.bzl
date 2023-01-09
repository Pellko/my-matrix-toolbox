def _impl_macos(repository_ctx):
  sdk_path = repository_ctx.os.environ.get("VULKAN_SDK", None)

  if sdk_path == "" or sdk_path == None:
    fail("Could not locate Vulkan SDK")
  
  repository_ctx.symlink(sdk_path, "vulkan_sdk_macos")

  vulkan_dep_build = """
cc_library(
  name = "vulkan",
  srcs = glob([
    "vulkan_sdk_macos/lib/libvulkan.dylib",
    "vulkan_sdk_macos/lib/libvulkan.1*.dylib",
  ]),
  hdrs = glob([
    "vulkan_sdk_macos/include/vulkan/**/*.h",
    "vulkan_sdk_macos/include/vulkan/**/*.hpp",
  ]),
  includes = ["vulkan_sdk_macos/include"],
  visibility = ["//visibility:public"],
)

filegroup(
  name = "glslc",
  srcs = ["vulkan_sdk_macos/bin/glslc"],
  visibility = ["//visibility:public"],
)
"""

  repository_ctx.file("BUILD.bazel", vulkan_dep_build)

vulkan_macos = repository_rule(
  implementation = _impl_macos,
  local = True,
  environ = ["VULKAN_SDK"],
)

def _glsl_toolchain_impl(ctx):
  glslc_executable = ctx.executable.glslc
  toolchain_info = platform_common.ToolchainInfo(
    glslc = ctx.attr.glslc,
    glslc_executable = glslc_executable
  )

  return [toolchain_info]

glsl_toolchain = rule(
  implementation = _glsl_toolchain_impl,
  attrs = {
    "glslc": attr.label(
      executable = True,
      cfg = "exec",
      allow_single_file = True,
      mandatory = True
    )
  }
)

def glsl_repositories():
  native.register_toolchains(
    "//bazel:glsl_macos_toolchain"
  )

def vulkan_repositories():
  vulkan_macos(
    name = "vulkan_macos"
  )

  glsl_repositories()