package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

custom_cc_library(
  name = "compiler",
  srcs = [
    "Compiler.cc",
    "Disassembler.cc",
    "Expression.cc",
    "Lexer.cc",
    "LexicalException.cc",
    "Statement.cc",
    "SyntaxException.cc",
  ],
  hdrs = [
    "Compiler.hh",
    "Disassembler.hh",
    "Expression.hh",
    "Lexer.hh",
    "LexicalException.hh",
    "Statement.hh",
    "SyntaxException.hh",
  ],
  deps = [
    "//src/vm:vm",
    "//src/types:types",
  ],
)