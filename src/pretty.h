#pragma once

#include "ast.h"
#include "common.h"

namespace tmonkey {

class AstPrettyfier {
public:
  static auto prettify(const StringInterningMap& strintern, const AstNode* n) -> std::string {
    ASSERT_NO_NULLPTR(n);

    AstPrettyfier p(strintern);
    p.visit(n);
    return p.result_;
  }

  static auto prettify(const StringInterningMap& strintern, const std::vector<AstNode*> tree)
      -> std::string {
    AstPrettyfier p(strintern);
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
  AstPrettyfier(const StringInterningMap& strintern) : strintern_{strintern} {}

  void visit(const AstNode* n) {
    ASSERT_NO_NULLPTR(n);

    indent_++;
    AST_NODE_SWITCH(AST_GEN_VISIT_NODE_SWITCH_CASE)
    indent_--;
  }

  AST_NODE_LIST(GEN_AST_VISITORS)

  int indent_ = 0;
  std::string result_;
  const StringInterningMap& strintern_;
};

}  // namespace tmonkey
