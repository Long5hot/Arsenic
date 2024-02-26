#include <string_view>
#include <iostream>
#include <Lex/lex.h>
#include <common/arsenic_error>

namespace arsenic {

// strTokenTypes is used for enum to string conversion.
// If you are adding any new TOKEN in TokenTypes, Do not forget
// to add same here as well.
// DO NOT TRY TO CHANGE THE ORDER OF BELOW ARRAY INDICES. #Hack

std::string_view strTokenTypes[] = {  // Single-character tokens
  "LEFT_PAREN",
  "RIGHT_PAREN",
  "LEFT_BRACE",
  "RIGHT_BRACE",
  "COMMA",
  "DOT",
  "MINUS",
  "PLUS",
  "SEMICOLON",
  "SLASH",
  "STAR",

  // One or two character tokens
  "BANG",
  "BANG_EQUAL",
  "EQUAL",
  "EQUAL_EQUAL",
  "GREATER",
  "GREATER_EQUAL",
  "LESS",
  "LESS_EQUAL",

  // Literals
  "IDENTIFIER",
  "STRING",
  "NUMBER",

  // KeyWords
  "AND",
  "OR",
  "IF",
  "ELSE",
  "CLASS",
  "TRUE",
  "FALSE",
  "FUN",
  "NIL",
  "PRINT",
  "RETURN",
  "SUPER",
  "VAR",
  "FOR",
  "WHILE",
  "THIS",

  "__EOF"
  };

bool Scanner::doesMatch(char expected) {
  if (isAtEnd())
    return false;
  if (source.at(current) != expected)
    return false;
  current++;
  return true;
}

void Scanner::scanString() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n')
      line++;
    advance();
  }

  if (isAtEnd()) {
    arsenicError.error(line, "Unterminated String.");
    return;
  }

  // The closing ".
  advance();

  // Trim the surrounding quotes.
  std::string Value = source.substr(start + 1, current - 1);
  addToken(STRING, Value);
}

void Scanner::identifier() {
  while (isAlphaNumeric(peek()))
    advance();
  std::string text = source.substr(start, current - start);
  auto it = keywords.find(text);
  if (it == keywords.end())
    addToken(IDENTIFIER);
  else
    addToken(it->second);
}

void Scanner::scanNumber() {
  while (isDigit(peek()))
    advance();

  // Look for a fractional part.
  if (peek() == '.' && isDigit(peekNext())) {
    // Consume the "."
    advance();
    while (isDigit(peek()))
      advance();
  }

  addToken(NUMBER, source.substr(start, current - start));
}

void Scanner::scanToken() {
  char c = advance();

  switch (c) {

  case '(':
    addToken(LEFT_PAREN);
    break;
  case ')':
    addToken(RIGHT_PAREN);
    break;
  case '{':
    addToken(LEFT_BRACE);
    break;
  case '}':
    addToken(RIGHT_BRACE);
    break;
  case ',':
    addToken(COMMA);
    break;
  case '.':
    addToken(DOT);
    break;
  case '-':
    addToken(MINUS);
    break;
  case '+':
    addToken(PLUS);
    break;
  case ';':
    addToken(SEMICOLON);
    break;
  case '*':
    addToken(STAR);
    break;
  case '!':
    addToken(doesMatch('=') ? BANG_EQUAL : BANG);
    break;
  case '=':
    addToken(doesMatch('=') ? EQUAL_EQUAL : EQUAL);
    break;
  case '<':
    addToken(doesMatch('=') ? LESS_EQUAL : LESS);
    break;
  case '>':
    addToken(doesMatch('=') ? GREATER_EQUAL : GREATER);
    break;
  case '/':
    if (doesMatch('/'))
      while (peek() != '\n' && !isAtEnd())
        advance();
    else
      addToken(SLASH);
    break;
  case ' ':
  case '\t':
  case '\r':
    break; // Ignore whitespace.
  case '\n':
    line++;
    break;

  case '"':
    scanString();
    break;

  default:
    if (isDigit(c))
      scanNumber();
    else if (isAlpha(c))
      identifier();
    else
      arsenicError.error(line, "Unexpected character " + c);
    break;
  }
}

std::list<Token> Scanner::scanTokens() {

  while (!isAtEnd()) {
    // We are at the beginning of the next lexeme.

    start = current;
    scanToken();
  }

  tokens.push_back(Token(__EOF, "", "End of File", line));

  dump();

  return tokens;
}

void Scanner::dump() {
  for (auto & it : tokens)
    std::cout << it.getLine() << " " << it.getLexeme() << " " << it.getLiteralValue() << " " << strTokenTypes[it.getType()] << std::endl;
}

} // namespace arsenic
