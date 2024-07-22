#include <Error/arsenic_error>
#include <Lex/Lex.h>

namespace arsenic {

void errorReporting::error(int line, const std::string &Message) {
  report(line, "", Message);
}

void errorReporting::error(Token token, const std::string &message) {
  if (token.getType() == __EOF__) {
    report(token.getLine(), " at end ", message);
  } else
    report(token.getLine(), " at '" + token.getLexeme() + "'", message);
}

void errorReporting::report(int line, std::string where, std::string Message) {
  std::cerr << red << "[line " << line << "]"
            << " Error " << where << ": " << reset << Message << std::endl;

  HadError = true;
}

bool errorReporting::hadError() { return HadError; }

void errorReporting::set() { HadError = true; }

void errorReporting::clear() { HadError = false; }

void InitializeErrorReporting() { arsenicError.clear(); }

} // namespace arsenic
