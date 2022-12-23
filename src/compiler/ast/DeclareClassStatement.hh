#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_DECLARE_CLASS_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_DECLARE_CLASS_STATEMENT_H_

#include "Statement.hh"

namespace mymatrixtoolbox {

struct ClassMethod {
  std::string name;
  int functionIndex;
  CompilerScope* compilerScope; // This pointer is not managed by this struct
  std::vector<Upvalue> upvalues;
  std::vector<Argument> arguments;
  Statement* block;
};

class DeclareClassStatement : public Statement {
 public:
  DeclareClassStatement(int classIndex, bool isGlobal) : Statement(StatementType::DECLARE_CLASS), isGlobal(isGlobal), classIndex(classIndex) {}
  ~DeclareClassStatement() {
    for(ClassMethod& m : methods) {
      delete m.block;
    }
  }

  void emitBytecode(Chunk& chunk) override;

  void addMethod(ClassMethod method) {
    methods.push_back(method);
  }
 private:
  bool isGlobal;
  int classIndex;
  std::vector<ClassMethod> methods;
};

}

#endif