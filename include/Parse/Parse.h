#ifndef __PARSE_H__
#define __PARSE_H__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Stmt/Stmt.h>
#include <initializer_list>
#include <memory>
#include <vector>

namespace arsenic {

struct ParserError {};

class Parser {

  std::vector<Token> tokens;
  int current = 0;

public:
  Parser(std::vector<Token> &tokens);
  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> equality();
  bool match(std::initializer_list<TokenType> types);
  bool check(TokenType type);
  Token advance();
  bool isAtEnd();
  Token peek();
  Token previous();
  std::unique_ptr<Expr> comparision();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> primary();
  std::unique_ptr<Expr> assignment();
  std::unique_ptr<Expr> logical_and();
  std::unique_ptr<Expr> logical_or();
  Token consume(TokenType type, const std::string &message);
  void synchronize();
  std::vector<std::unique_ptr<Stmt>> parse();
  ParserError error(Token token, const std::string &message);
  std::unique_ptr<Stmt> statement();
  std::unique_ptr<Stmt> printStatement();
  std::unique_ptr<Stmt> ifStatement();
  std::unique_ptr<Stmt> expressionStatement();
  std::unique_ptr<Stmt> declaration();
  std::unique_ptr<Stmt> varDeclaration();
  std::unique_ptr<Stmt> whileStatement();
  std::vector<std::unique_ptr<Stmt>> block();
};

} // namespace arsenic

#endif // __PARSE_H__
