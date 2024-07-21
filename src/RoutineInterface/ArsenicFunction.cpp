#include <Error/arsenic_runtime_error>
#include <RoutineInterface/ArsenicFunction.h>
#include <memory>

namespace arsenic {

ArsenicFunction::ArsenicFunction(FunctionStmt declaration, std::shared_ptr<Environment> closure)
    : declaration(declaration), closure(closure) {}

std::any ArsenicFunction::call(Interpreter interpreter,
                               std::vector<std::any> arguments) {

  std::shared_ptr<Environment> environment =
      std::make_shared<Environment>(closure);
  std::vector<Token> FunctionParameters = declaration.getParameters();

  for (int i = 0; i < declaration.parameterSize(); i++) {
    environment->define(FunctionParameters.at(i).getLexeme(), arguments.at(i));
  }

  try {
    interpreter.executeBlock(declaration.getFunctionBody(), environment);
  } catch (std::shared_ptr<Return_Exception> returnValue) {
    return returnValue->getReturnValue();
  }
  return {};
}

int ArsenicFunction::arity() { return declaration.parameterSize(); }

std::string ArsenicFunction::toString() {
  return "<fn " + declaration.getToken().getLexeme() + ">";
}

} // namespace arsenic
