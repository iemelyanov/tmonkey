#include "lexer.h"
#include "common.h"
#include "gtest/gtest.h"

namespace tmonkey {
namespace {

TEST(LexerTests, Basics) {
#define LEXER_ASSERT_EQ(LEXEME, TOK_KIND, TOK_TEXT) \
  do {                                              \
    Lexer lex(LEXEME);                              \
    auto tok = lex.next();                          \
    ASSERT_EQ(tok.kind(), Token::TOK_KIND);         \
    ASSERT_EQ(tok.text(), (TOK_TEXT));              \
  } while (0)

  for (auto s : {" ", "\t", "\n"}) {
    ASSERT_EQ(Lexer(s).next().kind(), Token::kEof);
  }

  LEXER_ASSERT_EQ(("+"), Token::kPlus, "+");
  LEXER_ASSERT_EQ(("-"), Token::kMinus, "-");
  LEXER_ASSERT_EQ(("*"), Token::kStar, "*");
  LEXER_ASSERT_EQ(("/"), Token::kSlash, "/");
  LEXER_ASSERT_EQ(("!"), Token::kBang, "!");
  LEXER_ASSERT_EQ(("!="), Token::kNotEq, "!=");
  LEXER_ASSERT_EQ(("="), Token::kEq, "=");
  LEXER_ASSERT_EQ(("=="), Token::kEqEq, "==");
  LEXER_ASSERT_EQ(("<"), Token::kLt, "<");
  LEXER_ASSERT_EQ(("<="), Token::kLtEq, "<=");
  LEXER_ASSERT_EQ((">"), Token::kGt, ">");
  LEXER_ASSERT_EQ((">="), Token::kGtEq, ">=");
  LEXER_ASSERT_EQ(("("), Token::kLParen, "(");
  LEXER_ASSERT_EQ((")"), Token::kRParen, ")");
  LEXER_ASSERT_EQ(("{"), Token::kLBrace, "{");
  LEXER_ASSERT_EQ(("}"), Token::kRBrace, "}");
  LEXER_ASSERT_EQ(("["), Token::kLBracket, "[");
  LEXER_ASSERT_EQ(("]"), Token::kRBracket, "]");
  LEXER_ASSERT_EQ((";"), Token::kSemicolon, ";");
  LEXER_ASSERT_EQ((","), Token::kComma, ",");
  LEXER_ASSERT_EQ(("."), Token::kDot, ".");
  LEXER_ASSERT_EQ("\"hello world\"", Token::kString, "hello world");
  LEXER_ASSERT_EQ("\"hello world", Token::kString, "hello world");
  LEXER_ASSERT_EQ("\"\"", Token::kString, "");
  LEXER_ASSERT_EQ("123", Token::kInteger, "123");
  LEXER_ASSERT_EQ("3.14", Token::kFloat, "3.14");
  LEXER_ASSERT_EQ("3.14.55", Token::kFloat, "3.14");
  LEXER_ASSERT_EQ("3...14", Token::kFloat, "3.");
  LEXER_ASSERT_EQ("3a", Token::kInteger, "3");
  LEXER_ASSERT_EQ("let", Token::kLet, "let");
  LEXER_ASSERT_EQ("return", Token::kReturn, "return");
  LEXER_ASSERT_EQ("fn", Token::kFn, "fn");
  LEXER_ASSERT_EQ("if", Token::kIf, "if");
  LEXER_ASSERT_EQ("else", Token::kElse, "else");
  LEXER_ASSERT_EQ("while", Token::kWhile, "while");
  LEXER_ASSERT_EQ("puts", Token::kPuts, "puts");
  LEXER_ASSERT_EQ("null", Token::kNull, "null");
  LEXER_ASSERT_EQ("import", Token::kImport, "import");
  LEXER_ASSERT_EQ("_ident_123", Token::kIdentifier, "_ident_123");
}

TEST(LexerLetTests, Basics) {
  Lexer lex("let _abc_123 = 123;");

  auto tok = lex.next();
  ASSERT_EQ(tok.kind(), Token::kLet);
  ASSERT_EQ(tok.text(), "let");

  tok = lex.next();
  ASSERT_EQ(tok.kind(), Token::kIdentifier);
  ASSERT_EQ(tok.text(), "_abc_123");

  tok = lex.next();
  ASSERT_EQ(tok.kind(), Token::kEq);
  ASSERT_EQ(tok.text(), "=");

  tok = lex.next();
  ASSERT_EQ(tok.kind(), Token::kInteger);
  ASSERT_EQ(tok.text(), "123");

  tok = lex.next();
  ASSERT_EQ(tok.kind(), Token::kSemicolon);
  ASSERT_EQ(tok.text(), ";");

  tok = lex.next();
  ASSERT_EQ(tok.kind(), Token::kEof);
}

TEST(LexerStringBuf1Tests, Basics) {
  Lexer lex("\"hello world\";");

  auto tok = lex.next();
  ASSERT_EQ(tok.kind(), Token::kString);

  tok = lex.next();
  ASSERT_EQ(tok.kind(), Token::kSemicolon);
}

}  // namespace
}  // namespace tmonkey