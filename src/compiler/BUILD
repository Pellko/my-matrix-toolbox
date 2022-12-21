package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

AST_SRCS = [
  "AssignVariableExpression.cc",
  "BinaryExpression.cc",
  "BlockStatement.cc",
  "CallExpression.cc",
  "DeclareVariableStatement.cc",
  "Expression.cc",
  "ExpressionStatement.cc",
  "ForStatement.cc",
  "FunctionStatement.cc",
  "IfStatement.cc",
  "LambdaExpression.cc",
  "PrimaryExpression.cc",
  "PrintStatement.cc",
  "ReturnStatement.cc",
  "Statement.cc",
  "UnaryExpression.cc",
]
AST_HDRS = [
  "AssignVariableExpression.hh",
  "BinaryExpression.hh",
  "BlockStatement.hh",
  "CallExpression.hh",
  "DeclareVariableStatement.hh",
  "Expression.hh",
  "ExpressionStatement.hh",
  "ForStatement.hh",
  "FunctionStatement.hh",
  "IfStatement.hh",
  "LambdaExpression.hh",
  "PrimaryExpression.hh",
  "PrintStatement.hh",
  "ReturnStatement.hh",
  "Statement.hh",
  "UnaryExpression.hh",
]

COMPILER_SRCS = [
  "Compiler.cc",
  "Disassembler.cc",
  "Lexer.cc",
  "LexicalException.cc",
  "ParserTool.cc",
  "SyntaxException.cc",
]

COMPILER_HDRS = [
  "Compiler.hh",
  "Disassembler.hh",
  "Lexer.hh",
  "LexicalException.hh",
  "ParserTool.hh",
  "SyntaxException.hh",
]

custom_cc_library(
  name = "compiler",
  srcs = COMPILER_SRCS + ["ast/" + f for f in AST_SRCS],
  hdrs = COMPILER_HDRS + ["ast/" + f for f in AST_HDRS],
  deps = [
    "//src/types:types",
  ],
)