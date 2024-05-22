#include <charconv>
#include <string_view>
#include <variant>
#include <iostream>
#include <Error/arsenic_error>
#include <Lex/Lex.h>

namespace arsenic {

// strTokenTypes is used for enum to string conversion.
// If you are adding any new TOKEN in TokenTypes, Do not forget
// to add same here as well.
// DO NOT TRY TO CHANGE THE ORDER OF BELOW ARRAY INDICES. #Hack

std::string_view strTokenTypes[] = { // Single-character tokens
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "COMMA", "DOT",
    "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",

    // One or two character tokens
    "BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL", "GREATER", "GREATER_EQUAL",
    "LESS", "LESS_EQUAL",

    // Literals
    "IDENTIFIER", "STRING", "NUMBER",

    // KeyWords
    "AND", "OR", "IF", "ELSE", "CLASS", "TRUE", "FALSE", "FUN", "NIL", "PRINT",
    "RETURN", "SUPER", "VAR", "FOR", "WHILE", "THIS",

    "__EOF__"};

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
  // 2 for starting " and closing"
  std::string Value = source.substr(start + 1, current - start - 2);
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
  double result;
  std::string tempString = source.substr(start, current - start);
  auto [ptr, ec] = std::from_chars(
      tempString.data(), tempString.data() + tempString.size(), result);
  if (ec == std::errc{})
    addToken(NUMBER, result);
  else
    arsenicError.error(line, "Incorrect Number");
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
      arsenicError.error(line, "Unexpected character " + std::string().append(1, c));
    break;
  }
}

std::vector<Token> Scanner::scanTokens() {

  while (!isAtEnd()) {
    // We are at the beginning of the next lexeme.

    start = current;
    scanToken();
  }

  tokens.push_back(Token(__EOF__, "", "End of File", line, start));

  dump();

  return tokens;
}

void Scanner::dump() {
  for (auto &it : tokens) {
    std::cout << it.getLine() << " " << it.getLexeme() << " "
              << it.getFileLocation() << " ";
    if (std::holds_alternative<std::string>(it.getLiteralValue()))
      std::cout << std::get<std::string>(it.getLiteralValue()) << " ";
    else if (std::holds_alternative<double>(it.getLiteralValue()))
      std::cout << std::get<double>(it.getLiteralValue()) << " ";
    std::cout << strTokenTypes[it.getType()] << std::endl;
  }
}

} // namespace arsenic
