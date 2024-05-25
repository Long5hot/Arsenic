#include <Error/arsenic_runtime_error>
#include <Lex/Lex.h>
#include <iostream>

namespace arsenic {

void runtimeError(RuntimeError &error) {
  std::cout << "[line " << error.getLine() << "] " << error.getLexemeString()
            << " " << error.getMessage() << std::endl;
}

} // namespace arsenic
