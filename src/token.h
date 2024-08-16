#pragma once

#include "common.h"

namespace tmonkey {

class Token {
public:
  enum Kind : uint8_t {
    kPlus,
    kMinus,
    kStar,
    kSlash,
    kBang,
    kNotEq,
    kEq,
    kEqEq,
    kLt,
    kLtEq,
    kGt,
    kGtEq,
    kLBrace,
    kRBrace,
    kLParen,
    kRParen,
    kLBracket,
    kRBracket,
    kColon,
    kSemicolon,
    kComma,
    kDot,
    kString,
    kInteger,
    kImport,
    kFloat,
    kTrue,
    kFalse,
    kNull,
    kIdentifier,
    kLet,
    kFn,
    kReturn,
    kPuts,
    kWhile,
    kIf,
    kElse,
    kEof
  };

  explicit Token(std::string_view text, Kind kind) : text_(text), kind_(kind) {}

  auto kind() const -> Kind {
    return kind_;
  }

  auto text() const -> std::string_view {
    return text_;
  }

private:
  std::string_view text_;
  Kind kind_ = kEof;
};

auto tokenKindStringify(Token::Kind kind) -> const char*;

}  // namespace tmonkey