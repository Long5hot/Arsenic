#include <list>
#include <string>
#include <string_view>
#include <unordered_map>

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

  __EOF
};

extern std::string_view strTokenTypes[];

class Token {

  TokenType type;
  std::string lexeme;
  std::string literal; // Literal Value
  int line;            // Line number information
  int location;        // Location from starting of source file

public:
  Token(TokenType type, std::string lexeme, std::string literal, int line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}

  std::string toString() { return lexeme; }

  TokenType getType() { return type; }
  std::string getLexeme() { return lexeme; }
  std::string getLiteralValue() { return literal; }
  int getLine() { return line; }
};

class Scanner {
  std::string source;
  std::list<Token> tokens;
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

  std::list<Token> scanTokens();

  void addToken(TokenType type, std::string literal = "") {
    std::string text = source.substr(start, start - current);
    tokens.push_back(Token(type, text, literal, line));
  }

  void dump();
};
} // namespace arsenic
