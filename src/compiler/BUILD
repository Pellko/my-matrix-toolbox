package(default_visibility = ["//visibility:public"])

load("//bazel:custom_library.bzl", "custom_cc_library")

AST_SRCS = [
  "AssignMatrixExpression.cc",
  "AssignPropertyExpression.cc",
  "AssignVariableExpression.cc",
  "BinaryExpression.cc",
  "BlockStatement.cc",
  "CallExpression.cc",
  "DeclareClassStatement.cc",
  "DeclareVariableStatement.cc",
  "Expression.cc",
  "ExpressionStatement.cc",
  "ForStatement.cc",
  "FunctionStatement.cc",
  "IfStatement.cc",
  "ImportModuleStatement.cc",
  "IncrementExpression.cc",
  "LambdaExpression.cc",
  "PrimaryExpression.cc",
  "PrintStatement.cc",
  "ReturnStatement.cc",
  "Statement.cc",
  "UnaryExpression.cc",
  "WhileStatement.cc",
]

AST_HDRS = [
  "AssignMatrixExpression.hh",
  "AssignPropertyExpression.hh",
  "AssignVariableExpression.hh",
  "BinaryExpression.hh",
  "BlockStatement.hh",
  "CallExpression.hh",
  "DeclareClassStatement.hh",
  "DeclareVariableStatement.hh",
  "Expression.hh",
  "ExpressionStatement.hh",
  "ForStatement.hh",
  "FunctionStatement.hh",
  "IfStatement.hh",
  "ImportModuleStatement.hh",
  "IncrementExpression.hh",
  "LambdaExpression.hh",
  "PrimaryExpression.hh",
  "PrintStatement.hh",
  "ReturnStatement.hh",
  "Statement.hh",
  "UnaryExpression.hh",
  "WhileStatement.hh",
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