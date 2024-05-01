#ifndef __PARSE_H__
#define __PARSE_H__

#include <Lex/Lex.h>
#include <initializer_list>
#include <memory>
#include <vector>
#include <Parse/Expr/Expr.h>

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
  Token consume(TokenType type, const std::string& message);
  void synchronize();
  std::unique_ptr<Expr> parse();
  ParserError error(Token token, const std::string& message);
};

} // namespace arsenic

#endif // __PARSE_H__
