#include "token.h"

namespace tmonkey {

auto tokenKindStringify(Token::Kind kind) -> const char* {
  switch (kind) {
    case Token::kPlus:
      return "+";
    case Token::kMinus:
      return "-";
    case Token::kStar:
      return "*";
    case Token::kSlash:
      return "/";
    case Token::kBang:
      return "!";
    case Token::kNotEq:
      return "!=";
    case Token::kEq:
      return "=";
    case Token::kEqEq:
      return "==";
    case Token::kLt:
      return "<";
    case Token::kLtEq:
      return "<=";
    case Token::kGt:
      return ">";
    case Token::kGtEq:
      return ">=";
    case Token::kLBrace:
      return "{";
    case Token::kRBrace:
      return "}";
    case Token::kLParen:
      return "(";
    case Token::kRParen:
      return ")";
    case Token::kLBracket:
      return "[";
    case Token::kRBracket:
      return "]";
    case Token::kColon:
      return ":";
    case Token::kSemicolon:
      return ";";
    case Token::kComma:
      return ",";
    case Token::kDot:
      return ".";
    case Token::kString:
      return "String";
    case Token::kInteger:
      return "Integer";
    case Token::kImport:
      return "Import";
    case Token::kFloat:
      return "Float";
    case Token::kTrue:
      return "True";
    case Token::kFalse:
      return "False";
    case Token::kNull:
      return "Null";
    case Token::kIdentifier:
      return "Identifier";
    case Token::kLet:
      return "Let";
    case Token::kFn:
      return "Fn";
    case Token::kReturn:
      return "Return";
    case Token::kPuts:
      return "Puts";
    case Token::kWhile:
      return "While";
    case Token::kIf:
      return "If";
    case Token::kElse:
      return "Else";
  }
  return "Eof";
}

}  // namespace tmonkey