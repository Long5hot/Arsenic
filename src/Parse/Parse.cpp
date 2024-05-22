#include <Lex/Lex.h>
#include <Parse/Expr/BinaryExpr.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/GroupingExpr.h>
#include <Parse/Expr/LiteralExpr.h>
#include <Parse/Expr/UnaryExpr.h>
#include <Parse/Parse.h>
#include <Error/arsenic_error>
#include <cstddef>
#include <initializer_list>
#include <memory>

namespace arsenic {

Parser::Parser(std::vector<Token> &tokens) : tokens(tokens) {}
// expression → equality ;
std::unique_ptr<Expr> Parser::expression() { return equality(); }

// equality → comparison ( ( "!=" | "==" ) comparison )* ;
std::unique_ptr<Expr> Parser::equality() {

  std::unique_ptr<Expr> expr = comparision();

  while (match({BANG_EQUAL, EQUAL_EQUAL})) {
    Token operator_t = previous();
    std::unique_ptr<Expr> right = comparision();
    expr = std::make_unique<BinaryExpr>(std::move(expr), operator_t,
                                        std::move(right));
  }

  return expr;
}

bool Parser::match(std::initializer_list<TokenType> types) {

  for (TokenType type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }

  return false;
}

bool Parser::check(TokenType type) {
  if (isAtEnd())
    return false;
  return peek().getType() == type;
}

Token Parser::advance() {
  if (!isAtEnd())
    current++;
  return previous();
}

bool Parser::isAtEnd() { return peek().getType() == __EOF__; }

Token Parser::peek() { return tokens.at(current); }

Token Parser::previous() { return tokens.at(current - 1); }

// comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
std::unique_ptr<Expr> Parser::comparision() {
  std::unique_ptr<Expr> expr = term();

  while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token operator_t = previous();
    std::unique_ptr<Expr> right = term();
    expr = std::make_unique<BinaryExpr>(std::move(expr), operator_t,
                                        std::move(right));
  }

  return expr;
}

// term → factor ( ( "-" | "+" ) factor )* ;
std::unique_ptr<Expr> Parser::term() {
  std::unique_ptr<Expr> expr = factor();

  while (match({MINUS, PLUS})) {
    Token operator_t = previous();
    std::unique_ptr<Expr> right = factor();
    expr = std::make_unique<BinaryExpr>(std::move(expr), operator_t,
                                        std::move(right));
  }

  return expr;
}

// factor → unary ( ( "/" | "*" ) unary )* ;
std::unique_ptr<Expr> Parser::factor() {
  std::unique_ptr<Expr> expr = unary();

  while (match({SLASH, STAR})) {
    Token operator_t = previous();
    std::unique_ptr<Expr> right = unary();
    expr = std::make_unique<BinaryExpr>(std::move(expr), operator_t,
                                        std::move(right));
  }

  return expr;
}

// unary → ( "!" | "-" ) unary | primary ;
std::unique_ptr<Expr> Parser::unary() {

  if (match({BANG, MINUS})) {
    Token operator_t = previous();
    std::unique_ptr<Expr> right = unary();
    return std::make_unique<UnaryExpr>(operator_t, std::move(right));
  }

  return primary();
}

// primary → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;

std::unique_ptr<Expr> Parser::primary() {

  if (match({FALSE}))
    return std::make_unique<LiteralExpr>(false);
  if (match({TRUE}))
    return std::make_unique<LiteralExpr>(true);
  if (match({NIL}))
    return std::make_unique<LiteralExpr>(nullptr);

  if (match({NUMBER, STRING})) {
    return std::make_unique<LiteralExpr>(previous().getLiteralValueAny());
  }
  if (match({LEFT_PAREN})) {
    std::unique_ptr<Expr> expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_unique<GroupingExpr>(std::move(expr));
  }

  throw error(peek(), "Expect expression.");
}

Token Parser::consume(TokenType type, const std::string &message) {

  if (check(type))
    return advance();
  throw error(peek(), message);
}

ParserError Parser::error(Token token, const std::string &message) {
  arsenicError.error(token, message);
  return ParserError();
}

void Parser::synchronize() {
  advance();

  while (!isAtEnd()) {
    if (previous().getType() == SEMICOLON)
      return;

    switch (peek().getType()) {
    case CLASS:
    case FUN:
    case VAR:
    case FOR:
    case IF:
    case WHILE:
    case PRINT:
    case RETURN:
      return;
    default:
      break;
    }

    advance();
  }
}

std::unique_ptr<Expr> Parser::parse() {
  try {
    return expression();
  } catch (ParserError error) {
    return nullptr;
  }
}

} // namespace arsenic
