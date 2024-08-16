#pragma once

#include "ast.h"
#include "common.h"

namespace tmonkey {

class AstPrettyfier {
public:
  static auto prettify(const AstNode* n) -> std::string {
    ASSERT_NO_NULLPTR(n);

    AstPrettyfier p;
    p.visit(n);
    return p.result_;
  }

  static auto prettify(const std::vector<AstNode*> tree) -> std::string {
    AstPrettyfier p;
    p.result_ += std::format("Ast {{\n");
    p.indent_++;
    for (auto* node : tree) {
      p.result_ += std::format("{:.>{}}", "", p.indent_);
      p.visit(node);
    }
    p.indent_--;
    p.result_ += std::format("}}\n");
    return p.result_;
  }

private:
  void visit(const AstNode* n) {
    ASSERT_NO_NULLPTR(n);

    indent_++;
    AST_NODE_SWITCH(AST_GEN_VISIT_NODE_SWITCH_CASE)
    indent_--;
  }

  AST_NODE_LIST(GEN_AST_VISITORS)

  int indent_ = 0;
  std::string result_;
};

}  // namespace tmonkey