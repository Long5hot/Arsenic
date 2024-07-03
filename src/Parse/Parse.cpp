#include <Error/arsenic_error>
#include <Lex/Lex.h>
#include <Parse/Expr/AssignExpr.h>
#include <Parse/Expr/BinaryExpr.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/GroupingExpr.h>
#include <Parse/Expr/LiteralExpr.h>
#include <Parse/Expr/LogicalExpr.h>
#include <Parse/Expr/UnaryExpr.h>
#include <Parse/Expr/VarExpr.h>
#include <Parse/Parse.h>
#include <Parse/Stmt/BlockStmt.h>
#include <Parse/Stmt/ExpressionStmt.h>
#include <Parse/Stmt/IfStmt.h>
#include <Parse/Stmt/PrintStmt.h>
#include <Parse/Stmt/Stmt.h>
#include <Parse/Stmt/VarStmt.h>
#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <vector>

namespace arsenic {

Parser::Parser(std::vector<Token> &tokens) : tokens(tokens) {}
// expression → equality ;

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

std::unique_ptr<Expr> Parser::expression() { return assignment(); }

std::unique_ptr<Expr> Parser::assignment() {

  std::unique_ptr<Expr> expr = logical_or();

  if (match({EQUAL})) {
    Token equals = previous();
    std::unique_ptr<Expr> value = assignment();

    if (VarExpr *var_expr = dynamic_cast<VarExpr *>(expr.get())) {
      return std::make_unique<AssignExpr>(var_expr->getToken(),
                                          std::move(value));
    }

    error(equals, "Invalid assignment target.");
  }

  return expr;
}

std::unique_ptr<Expr> Parser::logical_or() {

  std::unique_ptr<Expr> expr = logical_and();

  while (match({OR})) {
    Token operator_t = previous();
    std::unique_ptr<Expr> right = logical_and();
    expr = std::make_unique<LogicalExpr>(std::move(expr), operator_t,
                                         std::move(right));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::logical_and() {

  std::unique_ptr<Expr> expr = equality();

  while (match({AND})) {
    Token operator_t = previous();
    std::unique_ptr<Expr> right = equality();
    expr = std::make_unique<LogicalExpr>(std::move(expr), operator_t,
                                         std::move(right));
  }

  return expr;
}

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
    return std::make_unique<LiteralExpr>();

  if (match({NUMBER, STRING})) {
    return std::make_unique<LiteralExpr>(previous().getLiteralValueAny());
  }

  if (match({IDENTIFIER})) {
    return std::make_unique<VarExpr>(previous());
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

std::unique_ptr<Stmt> Parser::ifStatement() {

  consume(LEFT_PAREN, "Expect '(' after 'if'.");
  std::unique_ptr<Expr> condition = expression();
  consume(RIGHT_PAREN, "Expect ')' after if condition.");

  std::unique_ptr<Stmt> thenBranch = statement();
  std::unique_ptr<Stmt> elseBranch = nullptr;
  if (match({ELSE})) {
    elseBranch = statement();
  }
  return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch),
                                  std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::statement() {

  if (match({IF}))
    return ifStatement();

  if (match({PRINT}))
    return printStatement();

  if (match({LEFT_BRACE}))
    return std::make_unique<BlockStmt>(block());

  return expressionStatement();
}

std::vector<std::unique_ptr<Stmt>> Parser::block() {

  std::vector<std::unique_ptr<Stmt>> statements;

  while (!check(RIGHT_BRACE) && !isAtEnd()) {
    statements.push_back(declaration());
  }

  consume(RIGHT_BRACE, "Expect '}' after block.");
  return statements;
}

std::unique_ptr<Stmt> Parser::printStatement() {

  std::unique_ptr<Expr> value = expression();
  consume(SEMICOLON, "Expect ';' after value.");
  return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::expressionStatement() {

  std::unique_ptr<Expr> expr = expression();
  consume(SEMICOLON, "Expect ';' after value.");
  return std::make_unique<ExpressionStmt>(std::move(expr));
}

std::unique_ptr<Stmt> Parser::declaration() {

  try {
    if (match({VAR}))
      return varDeclaration();

    return statement();
  } catch (ParserError error) {
    synchronize();
    return nullptr;
  }
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
  Token name = consume(IDENTIFIER, "Expect variable name.");

  std::unique_ptr<Expr> initializer = nullptr;

  if (match({EQUAL})) {
    initializer = expression();
  }

  consume(SEMICOLON, "Expect ';' after variable declaration.");
  return std::make_unique<VarStmt>(name, std::move(initializer));
}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {

  std::vector<std::unique_ptr<Stmt>> statements;
  while (!isAtEnd()) {
    //    statements.push_back(statement());
    statements.push_back(declaration());
  }
  return statements;
}

} // namespace arsenic
