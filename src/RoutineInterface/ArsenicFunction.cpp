#include <Error/arsenic_runtime_error>
#include <Parse/Interpret.h>
#include <RoutineInterface/ArsenicFunction.h>
#include <memory>

namespace arsenic {

ArsenicFunction::ArsenicFunction(FunctionStmt declaration,
                                 std::shared_ptr<Environment> closure)
    : declaration(declaration), closure(closure) {}

ArsenicFunction::ArsenicFunction(const ArsenicFunction &A_Function)
    : declaration(A_Function.getFunctionDecl()),
      closure(A_Function.getClousure()) {}
//{
//  this->declaration = A_Function.getFunctionDecl();
//  this->closure = A_Function.getClousure();
//}

const FunctionStmt ArsenicFunction::getFunctionDecl() const {
  return declaration;
}

const std::shared_ptr<Environment> ArsenicFunction::getClousure() const {
  return closure;
}

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
