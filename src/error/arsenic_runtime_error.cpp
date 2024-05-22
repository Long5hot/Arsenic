#include <Lex/Lex.h>
#include <iostream>
#include <Error/arsenic_runtime_error>

namespace arsenic {

void runtimeError(RuntimeError& error) {
  std::cout << "[line " << error.getLine() << "] " << error.getLexemeString() << " " << error.getMessage() << std::endl;
}

}
