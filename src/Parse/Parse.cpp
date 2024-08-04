#include <Error/arsenic_error>
#include <Lex/Lex.h>
#include <Parse/Expr/AssignExpr.h>
#include <Parse/Expr/BinaryExpr.h>
#include <Parse/Expr/CallExpr.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/GetExpr.h>
#include <Parse/Expr/GroupingExpr.h>
#include <Parse/Expr/LiteralExpr.h>
#include <Parse/Expr/LogicalExpr.h>
#include <Parse/Expr/SetExpr.h>
#include <Parse/Expr/UnaryExpr.h>
#include <Parse/Expr/VarExpr.h>
#include <Parse/Parse.h>
#include <Parse/Stmt/BlockStmt.h>
#include <Parse/Stmt/ClassStmt.h>
#include <Parse/Stmt/ExpressionStmt.h>
#include <Parse/Stmt/FunctionStmt.h>
#include <Parse/Stmt/IfStmt.h>
#include <Parse/Stmt/PrintStmt.h>
#include <Parse/Stmt/ReturnStmt.h>
#include <Parse/Stmt/Stmt.h>
#include <Parse/Stmt/VarStmt.h>
#include <Parse/Stmt/WhileStmt.h>
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

std::shared_ptr<Expr> Parser::expression() { return assignment(); }

std::shared_ptr<Expr> Parser::assignment() {

  std::shared_ptr<Expr> expr = logical_or();

  if (match({EQUAL})) {
    Token equals = previous();
    std::shared_ptr<Expr> value = assignment();

    if (VarExpr *var_expr = dynamic_cast<VarExpr *>(expr.get())) {
      return std::make_shared<AssignExpr>(var_expr->getToken(),
                                          std::move(value));
    }

    if (GetExpr *get_expr = dynamic_cast<GetExpr *>(expr.get())) {
      return std::make_shared<SetExpr>(
          get_expr->getToken(), get_expr->getObjectPtr(), std::move(value));
    }

    error(equals, "Invalid assignment target.");
  }

  return expr;
}

std::shared_ptr<Expr> Parser::logical_or() {

  std::shared_ptr<Expr> expr = logical_and();

  while (match({OR})) {
    Token operator_t = previous();
    std::shared_ptr<Expr> right = logical_and();
    expr = std::make_shared<LogicalExpr>(std::move(expr), operator_t,
                                         std::move(right));
  }

  return expr;
}

std::shared_ptr<Expr> Parser::logical_and() {

  std::shared_ptr<Expr> expr = equality();

  while (match({AND})) {
    Token operator_t = previous();
    std::shared_ptr<Expr> right = equality();
    expr = std::make_shared<LogicalExpr>(std::move(expr), operator_t,
                                         std::move(right));
  }

  return expr;
}

// equality → comparison ( ( "!=" | "==" ) comparison )* ;
std::shared_ptr<Expr> Parser::equality() {

  std::shared_ptr<Expr> expr = comparision();

  while (match({BANG_EQUAL, EQUAL_EQUAL})) {
    Token operator_t = previous();
    std::shared_ptr<Expr> right = comparision();
    expr = std::make_shared<BinaryExpr>(std::move(expr), operator_t,
                                        std::move(right));
  }

  return expr;
}

// comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
std::shared_ptr<Expr> Parser::comparision() {
  std::shared_ptr<Expr> expr = term();

  while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token operator_t = previous();
    std::shared_ptr<Expr> right = term();
    expr = std::make_shared<BinaryExpr>(std::move(expr), operator_t,
                                        std::move(right));
  }

  return expr;
}

// term → factor ( ( "-" | "+" ) factor )* ;
std::shared_ptr<Expr> Parser::term() {
  std::shared_ptr<Expr> expr = factor();

  while (match({MINUS, PLUS})) {
    Token operator_t = previous();
    std::shared_ptr<Expr> right = factor();
    expr = std::make_shared<BinaryExpr>(std::move(expr), operator_t,
                                        std::move(right));
  }

  return expr;
}

// factor → unary ( ( "/" | "*" ) unary )* ;
std::shared_ptr<Expr> Parser::factor() {
  std::shared_ptr<Expr> expr = unary();

  while (match({SLASH, STAR})) {
    Token operator_t = previous();
    std::shared_ptr<Expr> right = unary();
    expr = std::make_shared<BinaryExpr>(std::move(expr), operator_t,
                                        std::move(right));
  }

  return expr;
}

// unary → ( "!" | "-" ) unary | primary ;
std::shared_ptr<Expr> Parser::unary() {

  if (match({BANG, MINUS})) {
    Token operator_t = previous();
    std::shared_ptr<Expr> right = unary();
    return std::make_shared<UnaryExpr>(operator_t, std::move(right));
  }

  return call();
}

std::shared_ptr<Expr> Parser::call() {

  std::shared_ptr<Expr> expr = primary();

  while (true) {
    if (match({TokenType::LEFT_PAREN})) {
      expr = finishCall(std::move(expr));
    } else if (match({TokenType::DOT})) {
      Token Name =
          consume(TokenType::IDENTIFIER, "Expected property name after '.'.");
      expr = std::make_shared<GetExpr>(Name, std::move(expr));
    } else
      break;
  }

  return expr;
}

std::shared_ptr<Expr> Parser::finishCall(std::shared_ptr<Expr> callee) {

  std::vector<std::shared_ptr<Expr>> arguments;

  if (!check(RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255)
        error(peek(), "Can't have more than 255 arguments");
      arguments.push_back(expression());
    } while (match({COMMA}));
  }

  Token closingParentheses =
      consume(RIGHT_PAREN, "Expected ')' after arguments");

  return std::make_unique<CallExpr>(std::move(callee), closingParentheses,
                                    std::move(arguments));
}

// primary → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;

std::shared_ptr<Expr> Parser::primary() {

  if (match({FALSE}))
    return std::make_shared<LiteralExpr>(false);
  if (match({TRUE}))
    return std::make_shared<LiteralExpr>(true);
  if (match({NIL}))
    return std::make_shared<LiteralExpr>();

  if (match({NUMBER, STRING})) {
    return std::make_shared<LiteralExpr>(previous().getLiteralValueAny());
  }

  if (match({IDENTIFIER})) {
    return std::make_shared<VarExpr>(previous());
  }

  if (match({LEFT_PAREN})) {
    std::shared_ptr<Expr> expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_shared<GroupingExpr>(std::move(expr));
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

std::shared_ptr<Stmt> Parser::ifStatement() {

  consume(LEFT_PAREN, "Expect '(' after 'if'.");
  std::shared_ptr<Expr> condition = expression();
  consume(RIGHT_PAREN, "Expect ')' after if condition.");

  std::shared_ptr<Stmt> thenBranch = statement();
  std::shared_ptr<Stmt> elseBranch = nullptr;
  if (match({ELSE})) {
    elseBranch = statement();
  }
  return std::make_shared<IfStmt>(std::move(condition), std::move(thenBranch),
                                  std::move(elseBranch));
}

std::shared_ptr<Stmt> Parser::returnStatement() {

  Token Keyword = previous();
  std::shared_ptr<Expr> Value;
  if (!check(TokenType::SEMICOLON))
    Value = expression();

  consume(TokenType::SEMICOLON, "Expect ';' after return value.");

  return std::make_shared<ReturnStmt>(Keyword, Value);
}

std::shared_ptr<Stmt> Parser::statement() {

  if (match({FOR}))
    return forStatement();

  if (match({IF}))
    return ifStatement();

  if (match({PRINT}))
    return printStatement();

  if (match({RETURN}))
    return returnStatement();

  if (match({WHILE}))
    return whileStatement();

  if (match({LEFT_BRACE}))
    return std::make_shared<BlockStmt>(block());

  return expressionStatement();
}

std::shared_ptr<Stmt> Parser::forStatement() {

  consume(LEFT_PAREN, "Expect '(' after 'for'.");

  std::shared_ptr<Stmt> initializer;

  // Check for initializer
  if (match({SEMICOLON}))
    initializer = nullptr;
  else if (match({VAR}))
    initializer = varDeclaration();
  else
    initializer = expressionStatement();

  std::shared_ptr<Expr> condition = nullptr;

  if (!check(SEMICOLON)) {
    condition = expression();
  }

  consume(SEMICOLON, "Expect ';' after loop condition.");

  std::shared_ptr<Expr> increment = nullptr;

  if (!check(RIGHT_PAREN)) {
    increment = expression();
  }

  consume(RIGHT_PAREN, "Expect ')' after for clauses.");

  std::shared_ptr<Stmt> forBody = statement();

  if (increment != nullptr) {
    std::vector<std::shared_ptr<Stmt>>
        forExprAndBody; // = {std::move(forBody),
                        // std::make_shared<ExpressionStmt>(std::move(increment))};
    forExprAndBody.push_back(std::move(forBody));
    forExprAndBody.push_back(
        std::make_shared<ExpressionStmt>(std::move(increment)));
    forBody = std::make_shared<BlockStmt>(std::move(forExprAndBody));
  }

  if (condition == nullptr)
    condition = std::make_shared<LiteralExpr>(true);

  forBody =
      std::make_shared<WhileStmt>(std::move(condition), std::move(forBody));

  if (initializer != nullptr) {
    std::vector<std::shared_ptr<Stmt>> forExprAndBody;
    forExprAndBody.push_back(std::move(initializer));
    forExprAndBody.push_back(std::move(forBody));
    // = {std::move(initializer), std::move(forBody)};
    forBody = std::make_shared<BlockStmt>(std::move(forExprAndBody));
  }
  return forBody;
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {

  std::vector<std::shared_ptr<Stmt>> statements;

  while (!check(RIGHT_BRACE) && !isAtEnd()) {
    statements.push_back(declaration());
  }

  consume(RIGHT_BRACE, "Expect '}' after block.");
  return statements;
}

std::shared_ptr<Stmt> Parser::printStatement() {

  std::shared_ptr<Expr> value = expression();
  consume(SEMICOLON, "Expect ';' after value.");
  return std::make_shared<PrintStmt>(std::move(value));
}

std::shared_ptr<Stmt> Parser::expressionStatement() {

  std::shared_ptr<Expr> expr = expression();
  consume(SEMICOLON, "Expect ';' after value.");
  return std::make_shared<ExpressionStmt>(std::move(expr));
}

std::shared_ptr<Stmt> Parser::whileStatement() {

  consume(LEFT_PAREN, "Expect '(' after 'while'.");
  std::shared_ptr<Expr> condition = expression();
  consume(RIGHT_PAREN, "Expect ')' after condition.");
  std::shared_ptr<Stmt> body = statement();

  return std::make_shared<WhileStmt>(std::move(condition), std::move(body));
}

std::shared_ptr<Stmt> Parser::function(std::string routineKind) {
  Token name =
      consume(TokenType::IDENTIFIER, "Expect " + routineKind + " name.");
  consume(LEFT_PAREN, "Expect '(' after " + routineKind + " name.");
  std::vector<Token> parameters;

  if (!check(TokenType::RIGHT_PAREN)) {
    do {
      if (parameters.size() >= 255)
        error(peek(), "Can't have more than 255 parameters.");

      parameters.push_back(
          consume(TokenType::IDENTIFIER, "Expect parameter name."));
    } while (match({TokenType::COMMA}));
  }

  consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

  consume(TokenType::LEFT_BRACE, "Expect '{' before " + routineKind + " body.");
  std::vector<std::shared_ptr<Stmt>> functionBody = block();
  return std::make_unique<FunctionStmt>(name, parameters,
                                        std::move(functionBody));
}

std::shared_ptr<Stmt> Parser::declaration() {

  try {
    if (match({CLASS}))
      return classDeclaration();
    if (match({FUN}))
      return function("function");
    if (match({VAR}))
      return varDeclaration();

    return statement();
  } catch (ParserError error) {
    synchronize();
    return nullptr;
  }
}

std::shared_ptr<Stmt> Parser::classDeclaration() {

  Token name = consume(IDENTIFIER, "Expect class name.");
  consume(LEFT_BRACE, "Expect '{' before class body.");

  std::vector<std::shared_ptr<Stmt>> methods;

  while (!check(RIGHT_BRACE) && !isAtEnd())
    methods.push_back(function("class method"));

  consume(RIGHT_BRACE, "Expect '}' after class body.");

  return std::make_shared<ClassStmt>(name, std::move(methods));
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
  Token name = consume(IDENTIFIER, "Expect variable name.");

  std::shared_ptr<Expr> initializer = nullptr;

  if (match({EQUAL})) {
    initializer = expression();
  }

  consume(SEMICOLON, "Expect ';' after variable declaration.");
  return std::make_shared<VarStmt>(name, std::move(initializer));
}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {

  std::vector<std::shared_ptr<Stmt>> statements;
  while (!isAtEnd()) {
    //    statements.push_back(statement());
    statements.push_back(declaration());
  }
  return statements;
}

} // namespace arsenic
