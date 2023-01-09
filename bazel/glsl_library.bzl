
def _glsl_library_impl(ctx):
  toolchain = ctx.toolchains["//bazel:glsl_toolchain_type"]
  shader = ctx.file.shader
  spirv_name = shader.basename + ".spv"
  spirv = ctx.actions.declare_file(spirv_name, sibling = shader)
  args = ctx.actions.args()
  inputs = [shader]

  args.add("-o", spirv)
  args.add(shader.path)

  ctx.actions.run(
    inputs = inputs,
    outputs = [spirv],
    arguments = [args],
    executable = toolchain.glslc_executable,
    progress_message = "Compiling GLSL",
    mnemonic = "glslc",
  )

  runfiles = ctx.runfiles(
    files = [spirv]
  )

  default_files = depset(direct = [spirv])

  return [
    DefaultInfo(files = default_files, runfiles = runfiles)
  ]

glsl_library = rule(
  implementation = _glsl_library_impl,
  attrs = {
    "shader": attr.label(allow_single_file = [".vert", ".frag", ".tesc", ".tese", ".geom", ".comp"]),
  },
  toolchains = ["//bazel:glsl_toolchain_type"],
  provides = [DefaultInfo]
)