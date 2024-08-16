#pragma once

#include "ast.h"
#include "common.h"

namespace tmonkey {
class Codegen {
public:
  void visit(const AstNode* n) {
    AST_NODE_SWITCH(AST_GEN_VISIT_NODE_SWITCH_CASE)
  }

private:
  AST_NODE_LIST(GEN_AST_VISITORS)
};

}  // namespace tmonkey