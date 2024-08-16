#include "codegen.h"

namespace tmonkey {

// void Codegen::visitBinExpr(AstNode* n) {
//   auto* e = static_cast<BinExpr*>(n);
//   visit(e->getLhs());
//   visit(e->getRhs());
// }

// void Codegen::visitUnaryExpr(AstNode* n) {
//   auto* e = static_cast<UnaryExpr*>(n);
// }

// void Codegen::visitIfExpr(AstNode* n) {
//   auto* e = static_cast<IfExpr*>(n);
// }

// void Codegen::visitFnExpr(AstNode* n) {
//   auto* e = static_cast<FnExpr*>(n);
// }

// void Codegen::visitCallExpr(AstNode* n) {
//   auto* e = static_cast<CallExpr*>(n);
// }

// void Codegen::visitLiteralExpr(AstNode* n) {
//   auto* e = static_cast<LiteralExpr*>(n);
// }

// void Codegen::visitArrayExpr(AstNode* n) {
//   auto* e = static_cast<ArrayExpr*>(n);
// }

// void Codegen::visitIndexExpr(AstNode* n) {
//   auto* e = static_cast<IndexExpr*>(n);
// }

// void Codegen::visitMapExpr(AstNode* n) {
//   auto* e = static_cast<MapExpr*>(n);
// }

// void Codegen::visitLetStmt(AstNode* n) {
//   auto* s = static_cast<LetStmt*>(n);
// }

// void Codegen::visitRetStmt(AstNode* n) {
//   auto* s = static_cast<RetStmt*>(n);
// }

// void Codegen::visitExprStmt(AstNode* n) {
//   auto* s = static_cast<ExprStmt*>(n);
//   // std::cout << std::format("{: >{}}-ExprStmt\n", "", indent_);
//   visit(s->getExpr());
// }

// void Codegen::visitBlockStmt(AstNode* n) {
//   auto* s = static_cast<BlockStmt*>(n);

//   // std::cout << std::format("{: >{}}-BlockStmt\n", "", indent_);
//   for (auto* s_ : s->getStmts()) {
//     visit(s_);
//   }
// }

}  // namespace tmonkey