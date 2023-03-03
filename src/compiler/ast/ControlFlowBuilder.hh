#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_CONTROL_FLOW_BUILDER_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_CONTROL_FLOW_BUILDER_H_

#include "src/types/Chunk.hh"
#include <vector>

namespace mymatrixtoolbox {

class ControlFlowBuilder {
 public:
  static ControlFlowBuilder begin();

  ControlFlowBuilder& addClause(std::function<void (Chunk& chunk)> condition, std::function<void (Chunk& chunk)> body);
  ControlFlowBuilder& addElseClause(std::function<void (Chunk& chunk)> body);
  void emitBytecode(Chunk& chunk);

 private:
  bool hasElse = false;
  std::function<void (Chunk& chunk)> elseBody;
  std::vector<std::pair<std::function<void (Chunk& chunk)>, std::function<void (Chunk& chunk)>>> clauses;
};
  
}

#endif