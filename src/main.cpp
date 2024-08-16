/*

Grammar

program        → declaration* EOF  ;

declaration    → letDecl | statement ;
letDecl        → "let" IDENTIFIER ( "=" expression )? ";" ;

statement      → exprStmt
               | ifStmt
               | returnStmt
               | whileStmt
               | block ;

exprStmt       → expression ";" ;
ifStmt         → "if" "(" expression ")" statement ( "else" statement )? ;
returnStmt     → "return" expression? ";" ;
whileStmt      → "while" "(" expression ")" statement ;
block          → "{" declaration* "}"  ;

expression     → assignment | call ;
assignment     → IDENTIFIER "=" assignment | call | equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )*  ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )*  ;
term           → factor ( ( "-" | "+" ) factor )*  ;
factor         → unary ( ( "/" | "*" ) unary )*  ;
unary          → ( "!" | "-" ) unary | call ;
call           → primary ( "(" arguments? ")" | "." IDENTIFIER )*  ;
primary        → "true" | "false" | NUMBER | STRING | IDENTIFIER | "(" expression ")";

function       → IDENTIFIER "(" parameters? ")" block ;
parameters     → IDENTIFIER ( "," IDENTIFIER )*  ;
arguments      → expression ( "," expression )*  ;

*/

#include "ast.h"
#include "codegen.h"
#include "common.h"
#include "parser.h"
#include "token.h"

int main(int argc, char* argv[]) {
  // auto tokPlus = Token("+", Token::kEof);
  // auto tok1 = Token("1", Token::kEof);
  // auto tok2 = Token("2", Token::kEof);

  // auto arena = Arena();

  // auto* litExpr1 = new (arena.alloc(sizeof(LiteralExpr))) LiteralExpr(tok1);
  // auto* litExpr2 = new (arena.alloc(sizeof(LiteralExpr))) LiteralExpr(tok2);

  // auto* binExpr1 = new (arena.alloc(sizeof(BinExpr))) BinExpr(tokPlus, litExpr1, litExpr2);
  // auto* binExpr2 = new (arena.alloc(sizeof(BinExpr))) BinExpr(tokPlus, litExpr1, litExpr2);

  // auto* exprStmt1 = new (arena.alloc(sizeof(ExprStmt))) ExprStmt(binExpr1);
  // auto* exprStmt2 = new (arena.alloc(sizeof(ExprStmt))) ExprStmt(binExpr2);

  // auto* retStmt = new(arena.alloc(sizeof(RetStmt))) RetStmt()

  // auto* blockStmt = new (arena.alloc(sizeof(BlockStmt))) BlockStmt({exprStmt1, exprStmt2});

  // print(blockStmt);

  // AstPrinter::print(blockStmt);

  return 0;
}