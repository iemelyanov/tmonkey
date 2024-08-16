#include "pretty.h"

namespace tmonkey {

void AstPrettyfier::visitExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  visit(n);
}

void AstPrettyfier::visitInfixExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const InfixExpr*>(n);
  result_ += std::format("InfixExpr {{\n");
  result_ += std::format("{:.>{}}Op: {}\n", "", indent_, tokenKindStringify(e->op.kind()));
  result_ += std::format("{:.>{}}Lhs: ", "", indent_);
  visit(e->lhs);
  result_ += std::format("{:.>{}}Rhs: ", "", indent_);
  visit(e->rhs);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitPrefixExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const PrefixExpr*>(n);
  result_ += std::format("InfixExpr {{\n");
  result_ += std::format("{:.>{}}Op: {}\n", "", indent_, tokenKindStringify(e->op.kind()));
  result_ += std::format("{:.>{}}Rhs: ", "", indent_);
  visit(e->rhs);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitIfExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const IfExpr*>(n);
  result_ += std::format("IfExpr {{\n");
  result_ += std::format("{:.>{}}Cnd: ", "", indent_);
  visit(e->cnd);
  result_ += std::format("{:.>{}}Coseq: ", "", indent_);
  visit(e->coseq);
  if (e->alt) {
    result_ += std::format("{:.>{}}Alt: ", "", indent_);
    visit(*e->alt);
  }
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitWhileExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const WhileExpr*>(n);
  result_ += std::format("WhileExpr {{\n");
  result_ += std::format("{:.>{}}Cnd: ", "", indent_);
  visit(e->cnd);
  result_ += std::format("{:.>{}}Coseq: ", "", indent_);
  visit(e->coseq);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitImportExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const ImportExpr*>(n);
}

void AstPrettyfier::visitFnExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const FnExpr*>(n);
  result_ += std::format("FnExpr {{\n");
  if (e->params.size() > 0) {
    result_ += std::format("{:.>{}}Params: [\n", "", indent_);
    for (auto* expr : e->params) {
      indent_++;
      result_ += std::format("{:.>{}}", "", indent_);
      visit(expr);
      indent_--;
    }
    result_ += std::format("{:.>{}}]\n", "", indent_);
  } else {
    result_ += std::format("{:.>{}}Params: []\n", "", indent_);
  }
  result_ += std::format("{:.>{}}Body: ", "", indent_);
  visit(e->body);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitCallExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const CallExpr*>(n);
  result_ += std::format("CallExpr {{\n");
  result_ += std::format("{:.>{}}Callable: ", "", indent_);
  visit(e->callable);
  result_ += std::format("{:.>{}}Arguments: [\n", "", indent_);
  for (auto* expr : e->args) {
    indent_++;
    result_ += std::format("{:.>{}}", "", indent_);
    visit(expr);
    indent_--;
  }
  result_ += std::format("{:.>{}}]\n", "", indent_);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitArrayExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const ArrayExpr*>(n);
  result_ += std::format("ArrayExpr {{\n");
  result_ += std::format("{:.>{}}Elements: [\n", "", indent_);
  for (auto* expr : e->elements) {
    indent_++;
    result_ += std::format("{:.>{}}", "", indent_);
    visit(expr);
    indent_--;
  }
  result_ += std::format("{:.>{}}]\n", "", indent_);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitAssignExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const AssignExpr*>(n);
  result_ += std::format("AssignExpr {{\n");
  result_ += std::format("{:.>{}}Lhs: ", "", indent_);
  visit(e->lhs);
  result_ += std::format("{:.>{}}Rhs: ", "", indent_);
  visit(e->rhs);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitIndexExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const IndexExpr*>(n);
  result_ += std::format("IndexExpr {{\n");
  result_ += std::format("{:.>{}}Lhs: ", "", indent_);
  visit(e->lhs);
  result_ += std::format("{:.>{}}Idx: ", "", indent_);
  visit(e->idx);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitHashMapExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const HashMapExpr*>(n);
  result_ += std::format("HashMapExpr {{\n");
  result_ += std::format("{:.>{}}Pairs: [\n", "", indent_);
  for (auto& p : e->pairs) {
    indent_++;
    result_ += std::format("{:.>{}}{{\n", "", indent_);
    indent_++;
    result_ += std::format("{:.>{}}Key: ", "", indent_);
    visit(p.first);
    result_ += std::format("{:.>{}}Val: ", "", indent_);
    visit(p.second);
    indent_--;
    result_ += std::format("{:.>{}}}}\n", "", indent_);
    indent_--;
  }
  result_ += std::format("{:.>{}}]\n", "", indent_);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitIdentifierExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const IdentifierExpr*>(n);
  result_ += std::format("IdentifierExpr {{\n");
  result_ += std::format("{:.>{}}Value: {}\n", "", indent_, e->identifier);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitNullExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const NullExpr*>(n);
  result_ += std::format("NullExpr {{}}\n");
}

void AstPrettyfier::visitBoolExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const BoolExpr*>(n);
  result_ += std::format("BoolExpr {{\n");
  result_ += std::format("{:.>{}}Value: {}\n", "", indent_, e->value);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitIntegerExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const IntegerExpr*>(n);
  result_ += std::format("IntegerExpr {{\n");
  result_ += std::format("{:.>{}}Value: {}\n", "", indent_, e->value);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitFloatExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const FloatExpr*>(n);
  result_ += std::format("FloatExpr {{\n");
  result_ += std::format("{:.>{}}Value: {}\n", "", indent_, e->value);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitStrExpr(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const StrExpr*>(n);
  result_ += std::format("StrExpr {{\n");
  result_ += std::format("{:.>{}}Value: '{}'\n", "", indent_, e->value);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitStmt(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* e = static_cast<const Stmt*>(n);
  // TODO: impl me
}

void AstPrettyfier::visitLetStmt(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* s = static_cast<const LetStmt*>(n);
  result_ += std::format("LetStmt {{\n");
  result_ += std::format("{:.>{}}Identifier: ", "", indent_);
  visit(s->identifier);
  result_ += std::format("{:.>{}}Expr: ", "", indent_);
  visit(s->rhs);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitRetStmt(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* s = static_cast<const RetStmt*>(n);
  result_ += std::format("RetStmt {{\n");
  result_ += std::format("{:.>{}}Expr: ", "", indent_);
  visit(s->expr);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitExprStmt(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* s = static_cast<const ExprStmt*>(n);
  result_ += std::format("ExprStmt {{\n");
  result_ += std::format("{:.>{}}", "", indent_);
  visit(s->expr);
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

void AstPrettyfier::visitBlockStmt(const AstNode* n) {
  ASSERT_NO_NULLPTR(n);

  const auto* s = static_cast<const BlockStmt*>(n);
  result_ += std::format("BlockStmt {{\n");
  if (s->body.size() > 0) {
    result_ += std::format("{:.>{}}Body: [\n", "", indent_);
    for (auto* stmt : s->body) {
      indent_++;
      result_ += std::format("{:.>{}}", "", indent_);
      visit(stmt);
      indent_--;
    }
    result_ += std::format("{:.>{}}]\n", "", indent_);
  } else {
    result_ += std::format("{:.>{}}Body: []\n", "", indent_);
  }
  result_ += std::format("{:.>{}}}}\n", "", indent_ - 1);
}

}  // namespace tmonkey