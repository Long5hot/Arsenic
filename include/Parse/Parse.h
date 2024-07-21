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
  bool match(std::initializer_list<TokenType> types);
  bool check(TokenType type);
  Token advance();
  bool isAtEnd();
  Token peek();
  Token previous();
  void synchronize();
  Token consume(TokenType type, const std::string &message);
  ParserError error(Token token, const std::string &message);
  std::shared_ptr<Expr> expression();
  std::shared_ptr<Expr> equality();
  std::shared_ptr<Expr> comparision();
  std::shared_ptr<Expr> term();
  std::shared_ptr<Expr> factor();
  std::shared_ptr<Expr> unary();
  std::shared_ptr<Expr> primary();
  std::shared_ptr<Expr> assignment();
  std::shared_ptr<Expr> logical_and();
  std::shared_ptr<Expr> logical_or();
  std::shared_ptr<Expr> call();
  std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee);
  std::shared_ptr<Stmt> function(std::string routineKind);
  std::shared_ptr<Stmt> statement();
  std::shared_ptr<Stmt> printStatement();
  std::shared_ptr<Stmt> ifStatement();
  std::shared_ptr<Stmt> expressionStatement();
  std::shared_ptr<Stmt> declaration();
  std::shared_ptr<Stmt> varDeclaration();
  std::shared_ptr<Stmt> whileStatement();
  std::shared_ptr<Stmt> forStatement();
  std::shared_ptr<Stmt> returnStatement();
  std::vector<std::shared_ptr<Stmt>> block();
  std::vector<std::shared_ptr<Stmt>> parse();
};

} // namespace arsenic

#endif // __PARSE_H__
