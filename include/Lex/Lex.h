#ifndef __LEXH__
#define __LEXH__

#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>
#include <any>

namespace arsenic {

// If you are planning to add new token then, do not forget to add
// the same in strTokenTypes of src/Lex/Lex.cpp

enum TokenType {

  // Single-character tokens
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals
  IDENTIFIER,
  STRING,
  NUMBER,

  // KeyWords
  AND,
  OR,
  IF,
  ELSE,
  CLASS,
  TRUE,
  FALSE,
  FUN,
  NIL,
  PRINT,
  RETURN,
  SUPER,
  VAR,
  FOR,
  WHILE,
  THIS,

  __EOF__
};

extern std::string_view strTokenTypes[];

using literal_variant = std::variant<std::monostate, std::string, double>;

class Token {

  TokenType type;
  std::string lexeme;
  literal_variant literal; // Literal Value
  int line;                // Line number information
  int location;            // Location from starting of source file

public:
  Token(TokenType type, std::string lexeme, literal_variant literal, int line,
        int location)
      : type(type), lexeme(lexeme), literal(literal), line(line),
        location(location) {}

  std::string toString() { return lexeme; }

  TokenType getType() { return type; }

  std::string getLexeme() { return lexeme; }
  literal_variant getLiteralValue() { return literal; }
  std::any getLiteralValueAny() {
    if (std::holds_alternative<std::string>(getLiteralValue()))
      return std::get<std::string>(getLiteralValue());
    else if (std::holds_alternative<double>(getLiteralValue()))
      return std::get<double>(getLiteralValue());
    else 
      return std::monostate();
  }
  int getLine() { return line; }
  int getFileLocation() { return location; }
};

class Scanner {
  std::string source;
  std::vector<Token> tokens;
  std::unordered_map<std::string, TokenType> keywords;
  int start = 0;
  int current = 0;
  int line = 1;
  int location = 0;

  void scanToken();
  void scanString();
  void scanNumber();
  void identifier();
  bool doesMatch(char);
  bool isAtEnd() { return current >= source.size(); }
  char advance() { return source.at(current++); }
  bool isDigit(char c) { return c >= '0' && c <= '9'; }
  bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }
  bool isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

  char peek() {
    if (isAtEnd())
      return '\0';
    return source.at(current);
  }

  char peekNext() {
    if (current + 1 >= source.size())
      return '\0';
    return source.at(current + 1);
  }

  void addToken(TokenType type, literal_variant literal = std::monostate{}) {
    std::string text = source.substr(start, current - start);
    tokens.push_back(Token(type, text, literal, line, start));
  }

public:
  Scanner(const char *BufferSource)
      : source(BufferSource), keywords({{"and", AND},
                                        {"class", CLASS},
                                        {"else", ELSE},
                                        {"false", FALSE},
                                        {"for", FOR},
                                        {"fun", FUN},
                                        {"if", IF},
                                        {"nil", NIL},
                                        {"or", OR},
                                        {"print", PRINT},
                                        {"return", RETURN},
                                        {"super", SUPER},
                                        {"this", THIS},
                                        {"true", TRUE},
                                        {"var", VAR},
                                        {"while", WHILE}}) {}

  std::vector<Token> scanTokens();

  void dump();
};
} // namespace arsenic

#endif // __LEXH__
