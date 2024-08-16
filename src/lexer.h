#pragma once

#include <unordered_map>
#include "common.h"
#include "token.h"

namespace tmonkey {

class Lexer {
public:
  Lexer(std::string_view text) : text_{text} {}

  NO_COPYABLE(Lexer);
  NO_MOVABLE(Lexer);

  auto next() -> Token;

private:
  auto isAtEnd() const -> bool {
    return curr_ == text_.size();
  }

  auto peek() -> char {
    if (isAtEnd()) {
      return '\0';
    }
    return text_[curr_];
  }

  auto advance() -> char {
    if (isAtEnd()) {
      return '\0';
    }
    return text_[curr_++];
  }

  auto makeToken(Token::Kind kind) const {
    return Token(text_.substr(start_, curr_ - start_), kind);
  }

  auto match(char c) -> bool {
    if (peek() == c) {
      advance();
      return true;
    }
    return false;
  }

  int start_ = -1;
  int curr_ = 0;
  std::string_view text_;

  std::unordered_map<std::string_view, Token::Kind> keywords_ = {
    {"let", Token::kLet},     {"return", Token::kReturn}, {"fn", Token::kFn},
    {"let", Token::kElse},    {"if", Token::kIf},         {"else", Token::kElse},
    {"while", Token::kWhile}, {"puts", Token::kPuts},     {"true", Token::kTrue},
    {"false", Token::kFalse}, {"null", Token::kNull},     {"import", Token::kImport},
  };
};

}  // namespace tmonkey