#include "parser.h"
#include <charconv>
#include "lexer.h"

namespace tmonkey {

enum PrecOrder : uint8_t {
  kPrecOrderAssign = 1,
  kPrecOrderEquals,
  kPrecOrderLeGt,
  kPrecOrderAddSub,
  kPrecOrderMulDiv,
  kPrecOrderPrefix,
  kPrecOrderCall,
  kPrecOrderIndex,
  kPrecOrderSelect,
};

auto precedence(Token::Kind kind) -> uint8_t {
  switch (kind) {
    case Token::kLet:
    case Token::kEq:
      return kPrecOrderAssign;
    case Token::kEqEq:
    case Token::kNotEq:
      return kPrecOrderEquals;
    case Token::kLt:
    case Token::kLtEq:
    case Token::kGt:
    case Token::kGtEq:
      return kPrecOrderLeGt;
    case Token::kPlus:
    case Token::kMinus:
      return kPrecOrderAddSub;
    case Token::kStar:
    case Token::kSlash:
      return kPrecOrderMulDiv;
    case Token::kLParen:
      return kPrecOrderCall;
    case Token::kLBracket:
      return kPrecOrderIndex;
    case Token::kDot:
      return kPrecOrderSelect;
    default:
      return 0;
  }
}

class Parser {
public:
  Parser(std::string_view source, Arena& arena) : arena_{arena}, lex_(source) {}

  auto parse() -> std::vector<AstNode*>;

private:
  void advance();
  auto matchPeek(Token::Kind kind) -> bool;

  auto parseInfixExpr(Expr* lhs) -> InfixExpr*;
  auto parsePrefixExpr() -> PrefixExpr*;
  auto parseIfExpr() -> IfExpr*;
  auto parseWhileExpr() -> WhileExpr*;
  auto parseImportExpr() -> ImportExpr*;
  auto parseFnExpr() -> FnExpr*;
  auto parseExprList(Token::Kind end) -> std::optional<std::vector<Expr*>>;
  auto parseCallExpr(Expr* callable) -> CallExpr*;
  auto parseArrayExpr() -> ArrayExpr*;
  auto parseAssignExpr(Expr* expr) -> AssignExpr*;
  auto parseIndexExpr(Expr* expr) -> IndexExpr*;
  auto parseHashMapExpr() -> HashMapExpr*;
  auto parseIdentifierExpr() -> IdentifierExpr*;
  auto parseNullExpr() -> NullExpr*;
  auto parseBoolExpr() -> BoolExpr*;
  auto parseIntegerExpr() -> IntegerExpr*;
  auto parseFloatExpr() -> FloatExpr*;
  auto parseStrExpr() -> StrExpr*;
  auto parseGroupExpr() -> Expr*;

  auto handlePrefixExpr(Token::Kind kind) -> Expr*;
  auto handleInfixExpr(Token::Kind kind, Expr* expr) -> Expr*;

  auto parseExpr(int prec) -> Expr*;

  auto parseLetStmt() -> LetStmt*;
  auto parseRetStmt() -> RetStmt*;
  auto parseBlockStmt() -> BlockStmt*;
  auto parseExprStmt() -> ExprStmt*;
  auto parseStmt() -> Stmt*;

  template <typename T, typename... Args>
  auto createNode(Args&&... args) {
    return new (arena_.alloc(sizeof(T))) T(std::forward<Args>(args)...);
  }

private:
  Arena& arena_;
  Lexer lex_;
  std::vector<std::string> errors_;

  Token curTok_ = Token("", Token::kEof);
  Token peekTok_ = Token("", Token::kEof);
};

#define LOG_PARSE_ERR(msg) errors_.push_back(std::format("parser error: {} {}", __func__, (msg)))

auto Parser::parse() -> std::vector<AstNode*> {
  // set peek and curr toks
  advance();
  advance();

  std::vector<AstNode*> tree;

  while (curTok_.kind() != Token::kEof) {
    if (auto* stmt = parseStmt(); stmt) {
      tree.push_back(stmt);
    }
    advance();
  }

  return tree;
}

void Parser::advance() {
  curTok_ = peekTok_;
  peekTok_ = lex_.next();
}

auto Parser::matchPeek(Token::Kind kind) -> bool {
  if (peekTok_.kind() == kind) {
    advance();
    return true;
  }
  errors_.push_back(std::format(
      "parser error: {} expected next token to be: {} got: {}", __func__, tokenKindStringify(kind),
      tokenKindStringify(peekTok_.kind())));
  return false;
}

auto Parser::parseExpr(int prec) -> Expr* {
  auto* lhs = handlePrefixExpr(curTok_.kind());
  if (!lhs) {
    return nullptr;
  }

  while (peekTok_.kind() != Token::kSemicolon && prec < precedence(peekTok_.kind())) {
    auto peekTokKind = peekTok_.kind();
    advance();
    lhs = handleInfixExpr(peekTokKind, lhs);
  }

  return lhs;
}

auto Parser::parseInfixExpr(Expr* lhs) -> InfixExpr* {
  auto tokOp = curTok_;
  advance();
  auto* rhs = parseExpr(precedence(tokOp.kind()));
  return createNode<InfixExpr>(tokOp, lhs, rhs);
}

auto Parser::parsePrefixExpr() -> PrefixExpr* {
  auto tokOp = curTok_;
  advance();
  auto* rhs = parseExpr(kPrecOrderPrefix);
  return createNode<PrefixExpr>(tokOp, rhs);
}

auto Parser::parseIfExpr() -> IfExpr* {
  if (!matchPeek(Token::kLParen)) {
    // TODO: produce err
    return nullptr;
  }

  auto* cndExpr = parseExpr(0);
  if (!cndExpr) {
    return nullptr;
  }

  if (!matchPeek(Token::kLBrace)) {
    // TODO: produce err
    return nullptr;
  }

  auto* coseqStmt = parseBlockStmt();
  if (!coseqStmt) {
    return nullptr;
  }

  BlockStmt* altStmt = nullptr;
  if (peekTok_.kind() == Token::kElse) {
    advance();

    if (!matchPeek(Token::kLBrace)) {
      // TODO: produce err
      return nullptr;
    }

    altStmt = parseBlockStmt();
    if (!altStmt) {
      return nullptr;
    }
  }

  return createNode<IfExpr>(
      cndExpr, coseqStmt, altStmt ? std::make_optional(altStmt) : std::nullopt);
}

auto Parser::parseWhileExpr() -> WhileExpr* {
  if (!matchPeek(Token::kLParen)) {
    // TODO: produce err
    return nullptr;
  }

  auto* cndExpr = parseExpr(0);
  if (!cndExpr) {
    return nullptr;
  }

  if (!matchPeek(Token::kLBrace)) {
    // TODO: produce err
    return nullptr;
  }

  auto* blockStmt = parseBlockStmt();
  if (!blockStmt) {
    return nullptr;
  }
  // TODO: fix case when expr stmt is empty

  return createNode<WhileExpr>(cndExpr, blockStmt);
}

auto Parser::parseImportExpr() -> ImportExpr* {
  return nullptr;
}

auto Parser::parseFnExpr() -> FnExpr* {
  if (!matchPeek(Token::kLParen)) {
    // TODO: produce err
    return nullptr;
  }

  auto argsExpr = parseExprList(Token::kRParen);
  if (!argsExpr) {
    return nullptr;
  }

  if (!matchPeek(Token::kLBrace)) {
    // TODO: produce err
    return nullptr;
  }

  auto* blockStmt = parseBlockStmt();
  if (!blockStmt) {
    return nullptr;
  }
  // TODO: fix case when expr stmt is empty

  return createNode<FnExpr>(*argsExpr, blockStmt);
}

auto Parser::parseExprList(Token::Kind end) -> std::optional<std::vector<Expr*>> {
  std::vector<Expr*> elements;

  while (peekTok_.kind() != end) {
    advance();

    auto* expr = parseExpr(0);
    if (!expr) {
      // TODO: produce err
      return {};
    }

    elements.push_back(expr);

    if (peekTok_.kind() != end && !matchPeek(Token::kComma)) {
      // TODO: produce err
      return {};
    }
  }

  if (!matchPeek(end)) {
    // TODO: produce err
    return {};
  }

  return elements;
}

auto Parser::parseCallExpr(Expr* callable) -> CallExpr* {
  auto elements = parseExprList(Token::kRParen);
  if (!elements) {
    return nullptr;
  }

  return createNode<CallExpr>(callable, *elements);
}

auto Parser::parseArrayExpr() -> ArrayExpr* {
  auto elements = parseExprList(Token::kRBracket);
  if (!elements) {
    return nullptr;
  }

  return createNode<ArrayExpr>(*elements);
}

auto Parser::parseAssignExpr(Expr* lhs) -> AssignExpr* {
  switch (lhs->kind()) {
    case Expr::Kind::kIdentifierExpr:
    case Expr::Kind::kIndexExpr:
      break;
    default:
      errors_.push_back(
          std::format("expected identifier or index expr on left but got {}", lhs->stringify()));
      return nullptr;
  }

  advance();
  auto* rhs = parseExpr(0);
  if (!rhs) {
    return nullptr;
  }

  return createNode<AssignExpr>(lhs, rhs);
}

auto Parser::parseIndexExpr(Expr* lhs) -> IndexExpr* {
  advance();
  auto* idxExpr = parseExpr(0);
  if (!matchPeek(Token::kRBracket)) {
    errors_.push_back(std::format("unmatched {}", tokenKindStringify(peekTok_.kind())));
    return nullptr;
  }
  return createNode<IndexExpr>(lhs, idxExpr);
}

auto Parser::parseHashMapExpr() -> HashMapExpr* {
  std::vector<std::pair<Expr*, Expr*>> pairs;

  while (peekTok_.kind() != Token::kRBrace) {
    advance();

    auto* keyExpr = parseExpr(0);
    if (!keyExpr) {
      // TODO: produce err
      return nullptr;
    }

    if (!matchPeek(Token::kColon)) {
      return nullptr;
    }

    advance();

    auto* valExpr = parseExpr(0);
    if (!keyExpr) {
      // TODO: produce err
      return nullptr;
    }

    pairs.push_back({keyExpr, valExpr});

    if (peekTok_.kind() != Token::kRBrace && !matchPeek(Token::kComma)) {
      // TODO: produce err
      return nullptr;
    }
  }

  if (!matchPeek(Token::kRBrace)) {
    // TODO: produce err
    return nullptr;
  }

  return createNode<HashMapExpr>(pairs);
}

auto Parser::parseIdentifierExpr() -> IdentifierExpr* {
  return createNode<IdentifierExpr>(curTok_.text());
}

auto Parser::parseNullExpr() -> NullExpr* {
  return createNode<NullExpr>();
}

auto Parser::parseBoolExpr() -> BoolExpr* {
  bool value = false;
  if (curTok_.kind() == Token::kTrue) {
    value = true;
  }
  return createNode<BoolExpr>(value);
}

auto Parser::parseIntegerExpr() -> IntegerExpr* {
  auto text = curTok_.text();
  int64_t val;
  auto res = std::from_chars(text.data(), text.data() + text.size(), val);
  if (res.ec != std::errc()) {
    LOG_PARSE_ERR(std::format("can't parse {} as integer", text.data()));
    return nullptr;
  }
  return createNode<IntegerExpr>(val);
}

auto Parser::parseFloatExpr() -> FloatExpr* {
  auto text = curTok_.text();
  char buf[64] = {0};
  memcpy(&buf[0], text.data(), text.size());
  char* end;
  double val = strtod(buf, &end);
  if (*end != '\0') {
    LOG_PARSE_ERR(std::format("can't parse {} as float", text.data()));
    return nullptr;
  }
  return createNode<FloatExpr>(val);
}

auto Parser::parseStrExpr() -> StrExpr* {
  return createNode<StrExpr>(curTok_.text());
}

auto Parser::parseGroupExpr() -> Expr* {
  advance();

  auto* expr = parseExpr(0);

  if (!matchPeek(Token::kRParen)) {
    return nullptr;
  }

  return expr;
}

auto Parser::parseStmt() -> Stmt* {
  switch (curTok_.kind()) {
    case Token::kReturn:
      return parseRetStmt();
    case Token::kLet:
      return parseLetStmt();
    default:
      return parseExprStmt();
  }
}

auto Parser::parseLetStmt() -> LetStmt* {
  if (!matchPeek(Token::kIdentifier)) {
    LOG_PARSE_ERR(std::format("unexpected token: {}", tokenKindStringify(curTok_.kind())));
    return nullptr;
  }

  auto* idStmt = parseIdentifierExpr();
  if (!idStmt) {
    return nullptr;
  }

  if (!matchPeek(Token::kEq)) {
    LOG_PARSE_ERR(std::format("unexpected token: {}", tokenKindStringify(curTok_.kind())));
    return nullptr;
  }

  advance();  // consume EQ

  auto* expr = parseExpr(0);
  if (!expr) {
    return nullptr;
  }

  if (peekTok_.kind() == Token::kSemicolon) {
    advance();
  }

  return createNode<LetStmt>(idStmt, expr);
}

auto Parser::parseRetStmt() -> RetStmt* {
  auto tok = curTok_;
  advance();
  auto* expr = parseExpr(0);
  if (peekTok_.kind() == Token::kSemicolon) {
    advance();
  }
  return createNode<RetStmt>(expr);
}

auto Parser::parseBlockStmt() -> BlockStmt* {
  std::vector<Stmt*> body;
  auto tok = curTok_;
  advance();
  while (curTok_.kind() != Token::kRBrace && curTok_.kind() != Token::kEof) {
    auto* stmt = parseStmt();
    if (!stmt) {
      // TODO: produce err
      return nullptr;
    }
    body.push_back(stmt);
    advance();
  }
  return createNode<BlockStmt>(body);
}

auto Parser::parseExprStmt() -> ExprStmt* {
  auto tok = curTok_;

  auto* expr = parseExpr(0);
  if (!expr) {
    // TODO: check this behaviour, possible error
    return nullptr;
  }

  if (peekTok_.kind() == Token::kSemicolon) {
    advance();
  }
  return createNode<ExprStmt>(expr);
}

#define LOG_UNKNOWN_TOK_HANDLE_ERR(tok_kind) \
  errors_.push_back(                         \
      std::format("parser error: {} unknown token: {}", __func__, tokenKindStringify(tok_kind)))

auto Parser::handlePrefixExpr(Token::Kind kind) -> Expr* {
  switch (kind) {
    case Token::kPuts:
    case Token::kIdentifier:
      return parseIdentifierExpr();
    case Token::kInteger:
      return parseIntegerExpr();
    case Token::kFloat:
      return parseFloatExpr();
    case Token::kString:
      return parseStrExpr();
    case Token::kMinus:
    case Token::kBang:
      return parsePrefixExpr();
    case Token::kTrue:
    case Token::kFalse:
      return parseBoolExpr();
    case Token::kNull:
      return parseNullExpr();
    case Token::kLParen:
      return parseGroupExpr();
    case Token::kIf:
      return parseIfExpr();
    case Token::kWhile:
      return parseWhileExpr();
    case Token::kImport:
      return parseImportExpr();
    case Token::kFn:
      return parseFnExpr();
    case Token::kLBracket:
      return parseArrayExpr();
    case Token::kLBrace:
      return parseHashMapExpr();
    default:
      LOG_UNKNOWN_TOK_HANDLE_ERR(kind);
      return nullptr;
  }
}

auto Parser::handleInfixExpr(Token::Kind kind, Expr* expr) -> Expr* {
  switch (kind) {
    case Token::kPlus:
    case Token::kMinus:
    case Token::kSlash:
    case Token::kStar:
    case Token::kEqEq:
    case Token::kNotEq:
    case Token::kLt:
    case Token::kLtEq:
    case Token::kGt:
    case Token::kGtEq:
      return parseInfixExpr(expr);
    case Token::kEq:
      return parseAssignExpr(expr);
    case Token::kLParen:
      return parseCallExpr(expr);
    case Token::kLBracket:
      return parseIndexExpr(expr);
    default:
      LOG_UNKNOWN_TOK_HANDLE_ERR(kind);
      return nullptr;
  }
}

auto parse(std::string_view source, Arena& arena) -> std::vector<AstNode*> {
  Parser parser(source, arena);
  return parser.parse();
}

}  // namespace tmonkey