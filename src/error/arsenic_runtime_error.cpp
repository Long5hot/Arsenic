#include <Error/arsenic_runtime_error>
#include <Lex/Lex.h>
#include <iostream>

namespace arsenic {

Return_Exception::Return_Exception(std::any Value) : Value(Value) {}

std::any Return_Exception::getReturnValue() { return Value; }

void runtimeError(RuntimeError &error) {
  std::cout << "\033[1;31m[line " << error.getLine() << "] Error: \033[0m"
            << error.getLexemeString() << " " << error.getMessage()
            << std::endl;
}

} // namespace arsenic
