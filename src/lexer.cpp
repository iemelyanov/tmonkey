#include "lexer.h"

namespace tmonkey {

auto isAlpha(char c) -> bool {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

auto isAlphaNumeric(char c) -> bool {
  return (c >= '0' && c <= '9') || isAlpha(c);
}

auto Lexer::next() -> Token {
  for (;;) {
    start_ = curr_;
    auto c = advance();

    switch (c) {
      case '\n':
      case '\t':
      case '\r':
      case ' ':
        continue;
      case '+':
        return makeToken(Token::kPlus);
      case '-':
        return makeToken(Token::kMinus);
      case '*':
        return makeToken(Token::kStar);
      case '/':
        return makeToken(Token::kSlash);
      case '=':
        return match('=') ? makeToken(Token::kEqEq) : makeToken(Token::kEq);
      case '!':
        return match('=') ? makeToken(Token::kNotEq) : makeToken(Token::kBang);
      case '<':
        return match('=') ? makeToken(Token::kLtEq) : makeToken(Token::kLt);
      case '>':
        return match('=') ? makeToken(Token::kGtEq) : makeToken(Token::kGt);
      case '{':
        return makeToken(Token::kLBrace);
      case '}':
        return makeToken(Token::kRBrace);
      case '(':
        return makeToken(Token::kLParen);
      case ')':
        return makeToken(Token::kRParen);
      case '[':
        return makeToken(Token::kLBracket);
      case ']':
        return makeToken(Token::kRBracket);
      case ':':
        return makeToken(Token::kColon);
      case ';':
        return makeToken(Token::kSemicolon);
      case ',':
        return makeToken(Token::kComma);
      case '.':
        return makeToken(Token::kDot);
      case '"': {
        while (peek() != '"') {
          char c = advance();
          if (c == '\0') {
            break;
          }
        }
        auto offset = 1;
        if (peek() == '"') {
          advance();
          offset = 2;
        }
        auto substr = text_.substr(start_ + 1, curr_ - start_ - offset);
        return Token(substr, Token::kString);
      }
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        auto seeDot = false;
        for (;;) {
          auto c = peek();
          if (c >= '0' && c <= '9') {
            curr_++;
            continue;
          }
          if (c == '.') {
            if (seeDot) {
              break;
            }
            seeDot = true;
            curr_++;
            continue;
          }
          break;
        }
        auto substr = text_.substr(start_, curr_ - start_);
        if (seeDot) {
          return Token(substr, Token::kFloat);
        } else {
          return Token(substr, Token::kInteger);
        }
      }
      default: {
        if (!(isAlpha(c) || c == '_')) {
          return Token("", Token::kEof);
        }
        while (isAlphaNumeric(peek()) || peek() == '_') {
          advance();
        }
        auto substr = text_.substr(start_, curr_ - start_);
        if (auto it = keywords_.find(substr); it != keywords_.end()) {
          return Token(substr, it->second);
        }
        return Token(substr, Token::kIdentifier);
      }
    }
  }

  return Token("", Token::kEof);
}

}  // namespace tmonkey