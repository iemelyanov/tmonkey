#pragma once

#include "common.h"
#include "token.h"

namespace tmonkey {
#define AST_NODE_LIST(V) \
  V(Expr)                \
  V(InfixExpr)           \
  V(PrefixExpr)          \
  V(IfExpr)              \
  V(WhileExpr)           \
  V(ImportExpr)          \
  V(FnExpr)              \
  V(CallExpr)            \
  V(ArrayExpr)           \
  V(AssignExpr)          \
  V(IndexExpr)           \
  V(HashMapExpr)         \
  V(IdentifierExpr)      \
  V(NullExpr)            \
  V(BoolExpr)            \
  V(IntegerExpr)         \
  V(FloatExpr)           \
  V(StrExpr)             \
  V(Stmt)                \
  V(LetStmt)             \
  V(RetStmt)             \
  V(BlockStmt)           \
  V(ExprStmt)

#define AST_GEN_VISIT_NODE_SWITCH_CASE(Name)  \
  case AstNode::Kind::k##Name:                \
    visit##Name(static_cast<const Name*>(n)); \
    break;

#define AST_NODE_SWITCH(GEN_CASE) \
  switch (n->kind()) {            \
    AST_NODE_LIST(GEN_CASE)       \
    default:                      \
      break;                      \
  }

#define GEN_AST_VISITORS(Name) void visit##Name(const AstNode* n);

class BlockStmt;

class AstNode {
public:
  enum class Kind {
#define GEN_KINDS(Name) k##Name,
    AST_NODE_LIST(GEN_KINDS)
#undef GEN_KINDS
  };

  AstNode(Kind kind) : kind_{kind} {}

  NO_COPYABLE(AstNode);
  NO_MOVABLE(AstNode);

  auto kind() const -> Kind {
    return kind_;
  }

  auto stringify() const -> const char* {
#define GEN_STR_NODE_KIND_SWITCH_CASE(Name) \
  case AstNode::Kind::k##Name:              \
    return "##Name";                        \
    break;

    auto* n = this;
    AST_NODE_SWITCH(GEN_STR_NODE_KIND_SWITCH_CASE)
    return "";

#undef GEN_STR_NODE_KIND_SWITCH_CASE
  }

private:
  Kind kind_;
};

class Expr : public AstNode {
public:
  Expr(Kind kind) : AstNode(kind) {}
};

class Stmt : public AstNode {
public:
  Stmt(Kind kind) : AstNode(kind) {}
};

class InfixExpr final : public Expr {
public:
  explicit InfixExpr(Token op, const Expr* lhs, const Expr* rhs)
      : Expr(Kind::kInfixExpr), op(op), lhs(lhs), rhs(rhs) {
    ASSERT_NO_NULLPTR(lhs);
    ASSERT_NO_NULLPTR(rhs);
  }

  const Token op;
  const Expr* lhs;
  const Expr* rhs;
};

class PrefixExpr final : public Expr {
public:
  explicit PrefixExpr(Token op, const Expr* rhs) : Expr(Kind::kPrefixExpr), op(op), rhs(rhs) {
    ASSERT_NO_NULLPTR(rhs)
  }

  const Token op;
  const Expr* rhs;
};

class IfExpr final : public Expr {
public:
  explicit IfExpr(const Expr* cnd, const BlockStmt* coseq, std::optional<const BlockStmt*> alt)
      : Expr(Kind::kIfExpr), cnd(cnd), coseq(coseq), alt(alt) {
    ASSERT_NO_NULLPTR(cnd);
    ASSERT_NO_NULLPTR(coseq);
  }

  const Expr* cnd;
  const BlockStmt* coseq;
  const std::optional<const BlockStmt*> alt;
};

class WhileExpr final : public Expr {
public:
  explicit WhileExpr(const Expr* cnd, const BlockStmt* coseq)
      : Expr(Kind::kWhileExpr), cnd(cnd), coseq(coseq) {
    ASSERT_NO_NULLPTR(cnd);
    ASSERT_NO_NULLPTR(coseq);
  }

  const Expr* cnd;
  const BlockStmt* coseq;
};

class ImportExpr final : public Expr {
public:
  explicit ImportExpr(const Expr* name) : Expr(Kind::kImportExpr), name(name) {
    ASSERT_NO_NULLPTR(name);
  }

  const Expr* name;
};

class ExprStmt final : public Stmt {
public:
  ExprStmt(const Expr* expr) : Stmt(Kind::kExprStmt), expr(expr) {
    ASSERT_NO_NULLPTR(expr);
  }

  const Expr* expr;
};

class FnExpr final : public Expr {
public:
  explicit FnExpr(const std::vector<Expr*>& params, const BlockStmt* body)
      : Expr(Kind::kFnExpr), params(params), body(body) {
    ASSERT_NO_NULLPTR(body);
  }

  const std::vector<Expr*> params;
  const BlockStmt* body;
};

class CallExpr final : public Expr {
public:
  explicit CallExpr(const Expr* callable, const std::vector<Expr*>& args)
      : Expr(Kind::kCallExpr), callable(callable), args(args) {
    ASSERT_NO_NULLPTR(callable);
  }

  const Expr* callable;
  const std::vector<Expr*> args;
};

class ArrayExpr final : public Expr {
public:
  explicit ArrayExpr(const std::vector<Expr*>& elements)
      : Expr(Kind::kArrayExpr), elements(elements) {}

  const std::vector<Expr*> elements;
};

class AssignExpr final : public Expr {
public:
  explicit AssignExpr(Expr* lhs, Expr* rhs) : Expr(Kind::kAssignExpr), lhs(lhs), rhs(rhs) {}

  Expr* lhs;
  Expr* rhs;
};

class IndexExpr final : public Expr {
public:
  explicit IndexExpr(Expr* lhs, Expr* idx) : Expr(Kind::kIndexExpr), lhs(lhs), idx(idx) {
    ASSERT_NO_NULLPTR(lhs);
    ASSERT_NO_NULLPTR(idx);
  }

  const Expr* lhs;
  const Expr* idx;
};

class HashMapExpr final : public Expr {
public:
  explicit HashMapExpr(const std::vector<std::pair<Expr*, Expr*>>& pairs)
      : Expr(Kind::kHashMapExpr), pairs(pairs) {}

  const std::vector<std::pair<Expr*, Expr*>> pairs;
};

class IdentifierExpr final : public Expr {
public:
  IdentifierExpr(std::string_view identifier)
      : Expr(Kind::kIdentifierExpr), identifier(identifier) {}

  const std::string_view identifier;
};

class NullExpr final : public Expr {
public:
  NullExpr() : Expr(Kind::kNullExpr) {}
};

class BoolExpr final : public Expr {
public:
  BoolExpr(bool value) : Expr(Kind::kBoolExpr), value{value} {}

  const bool value;
};

class IntegerExpr final : public Expr {
public:
  IntegerExpr(int64_t value) : Expr(Kind::kIntegerExpr), value{value} {}

  const int64_t value;
};

class FloatExpr final : public Expr {
public:
  FloatExpr(double value) : Expr(Kind::kFloatExpr), value{value} {}

  const double value;
};

class StrExpr final : public Expr {
public:
  StrExpr(std::string_view value) : Expr(Kind::kStrExpr), value{value} {}

  const std::string_view value;
};

class LetStmt final : public Stmt {
public:
  explicit LetStmt(IdentifierExpr* identifier, const Expr* rhs)
      : Stmt(Kind::kLetStmt), identifier(identifier), rhs(rhs) {
    ASSERT_NO_NULLPTR(identifier);
    ASSERT_NO_NULLPTR(rhs);
  }

  const IdentifierExpr* identifier;
  const Expr* rhs;
};

class RetStmt final : public Stmt {
public:
  explicit RetStmt(const Expr* expr) : Stmt(Kind::kRetStmt), expr(expr) {
    ASSERT_NO_NULLPTR(expr);
  }

  const Expr* expr;
};

class BlockStmt final : public Stmt {
public:
  BlockStmt(const std::vector<Stmt*>& body) : Stmt(Kind::kBlockStmt), body(body) {}

  const std::vector<Stmt*> body;
};

}  // namespace tmonkey