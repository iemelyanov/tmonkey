#include "parser.h"
#include "common.h"
#include "gtest/gtest.h"
#include "pretty.h"

namespace tmonkey {
namespace {

#define TM_ASSERT_TREE(expected, prog)                          \
  do {                                                          \
    tmonkey::Arena arena;                                       \
    std::vector<AstNode*> tree = tmonkey::parse((prog), arena); \
    ASSERT_TRUE(tree.size() > 0);                               \
    auto str = tmonkey::AstPrettyfier::prettify(tree);          \
    ASSERT_EQ(expected, str);                                   \
  } while (0)

TEST(ParserTests, Integer) {
  std::string prog = "123";
  std::string expected = R"""(Ast {
.ExprStmt {
..IntegerExpr {
...Value: 123
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Float) {
  std::string prog = "3.14";
  std::string expected = R"""(Ast {
.ExprStmt {
..FloatExpr {
...Value: 3.14
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Bool) {
  std::string prog = "true\nfalse";
  std::string expected = R"""(Ast {
.ExprStmt {
..BoolExpr {
...Value: true
..}
.}
.ExprStmt {
..BoolExpr {
...Value: false
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, String) {
  std::string prog = "\"my string\"";
  std::string expected = R"""(Ast {
.ExprStmt {
..StrExpr {
...Value: 'my string'
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Null) {
  std::string prog = "null";
  std::string expected = R"""(Ast {
.ExprStmt {
..NullExpr {}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Precedence) {
  std::string prog = "1 + 2 * 3";
  std::string expected = R"""(Ast {
.ExprStmt {
..InfixExpr {
...Op: +
...Lhs: IntegerExpr {
....Value: 1
...}
...Rhs: InfixExpr {
....Op: *
....Lhs: IntegerExpr {
.....Value: 2
....}
....Rhs: IntegerExpr {
.....Value: 3
....}
...}
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Group) {
  std::string prog = "(1 + 2) * 3";
  std::string expected = R"""(Ast {
.ExprStmt {
..InfixExpr {
...Op: *
...Lhs: InfixExpr {
....Op: +
....Lhs: IntegerExpr {
.....Value: 1
....}
....Rhs: IntegerExpr {
.....Value: 2
....}
...}
...Rhs: IntegerExpr {
....Value: 3
...}
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, InfixUnary) {
  std::string prog = "-1";
  std::string expected = R"""(Ast {
.ExprStmt {
..InfixExpr {
...Op: -
...Rhs: IntegerExpr {
....Value: 1
...}
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Let) {
  std::string prog = "let a = 1;";
  std::string expected = R"""(Ast {
.LetStmt {
..Identifier: IdentifierExpr {
...Value: a
..}
..Expr: IntegerExpr {
...Value: 1
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Index) {
  std::string prog = "foo[1]";
  std::string expected = R"""(Ast {
.ExprStmt {
..IndexExpr {
...Lhs: IdentifierExpr {
....Value: foo
...}
...Idx: IntegerExpr {
....Value: 1
...}
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, AssignToIndentifier) {
  std::string prog = "a = 1;";
  std::string expected = R"""(Ast {
.ExprStmt {
..AssignExpr {
...Lhs: IdentifierExpr {
....Value: a
...}
...Rhs: IntegerExpr {
....Value: 1
...}
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, AssignToIndex) {
  std::string prog = "a[123] = 1;";
  std::string expected = R"""(Ast {
.ExprStmt {
..AssignExpr {
...Lhs: IndexExpr {
....Lhs: IdentifierExpr {
.....Value: a
....}
....Idx: IntegerExpr {
.....Value: 123
....}
...}
...Rhs: IntegerExpr {
....Value: 1
...}
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Return) {
  std::string prog = "return 1;";
  std::string expected = R"""(Ast {
.RetStmt {
..Expr: IntegerExpr {
...Value: 1
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Call) {
  std::string prog = "a(1, \"foo\")";
  std::string expected = R"""(Ast {
.ExprStmt {
..CallExpr {
...Callable: IdentifierExpr {
....Value: a
...}
...Arguments: [
....IntegerExpr {
.....Value: 1
....}
....StrExpr {
.....Value: 'foo'
....}
...]
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Fn) {
  std::string prog = "let f = fn(){}";
  std::string expected = R"""(Ast {
.LetStmt {
..Identifier: IdentifierExpr {
...Value: f
..}
..Expr: FnExpr {
...Params: []
...Body: BlockStmt {
....Body: []
...}
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, While) {
  std::string prog = "while (a) {}";
  std::string expected = R"""(Ast {
.ExprStmt {
..WhileExpr {
...Cnd: IdentifierExpr {
....Value: a
...}
...Coseq: BlockStmt {
....Body: []
...}
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, If) {
  std::string prog = "if (a) {} else {}";
  std::string expected = R"""(Ast {
.ExprStmt {
..IfExpr {
...Cnd: IdentifierExpr {
....Value: a
...}
...Coseq: BlockStmt {
....Body: []
...}
...Alt: BlockStmt {
....Body: []
...}
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Array) {
  std::string prog = "[1,2,3]";
  std::string expected = R"""(Ast {
.ExprStmt {
..ArrayExpr {
...Elements: [
....IntegerExpr {
.....Value: 1
....}
....IntegerExpr {
.....Value: 2
....}
....IntegerExpr {
.....Value: 3
....}
...]
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, HashMap) {
  std::string prog = "{\"key1\": 1, 2: \"val\"}";
  std::string expected = R"""(Ast {
.ExprStmt {
..HashMapExpr {
...Pairs: [
....{
.....Key: StrExpr {
......Value: 'key1'
.....}
.....Val: IntegerExpr {
......Value: 1
.....}
....}
....{
.....Key: IntegerExpr {
......Value: 2
.....}
.....Val: StrExpr {
......Value: 'val'
.....}
....}
...]
..}
.}
}
)""";
  TM_ASSERT_TREE(expected, prog);
}

TEST(ParserTests, Prog) {
  std::string prog = R"""(
let fibo = fn(x) {
  if (x <= 1) {
    return x;
  }
  return fibo(x - 1) + fibo(x - 2);
};

puts(fibo(20));
)""";
  std::string expected = R"""()""";
  TM_ASSERT_TREE(expected, prog);
}

}  // namespace
}  // namespace tmonkey