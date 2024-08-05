#include <Parse/Stmt/FunctionStmt.h>

namespace arsenic {

FunctionStmt::FunctionStmt(Token name, std::vector<Token> parameters,
                           std::vector<std::shared_ptr<Stmt>> functionBody)
    : name(name), parameters(parameters),
      functionBody(std::move(functionBody)) {}

FunctionStmt::FunctionStmt(FunctionStmt &object)
    : name(object.getToken()), parameters(object.getParameters()),
      functionBody(object.getFunctionBody()) {
  //  this->parameters = object.getParameters();
  //
  //  unsigned int loopCount = object.getFunctionBody().size();
  //
  //  for (int i=0; i < loopCount; ++i) {
  //    functionBody.push_back(object.getFunctionBody().at(i));
  //  }
}

FunctionStmt::FunctionStmt(const FunctionStmt &object)
    : name(object.getToken()), parameters(object.getParameters()),
      functionBody(object.getFunctionBody()) {}

std::any FunctionStmt::accept(StmtVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

int FunctionStmt::parameterSize() { return parameters.size(); }

std::vector<Token> FunctionStmt::getParameters() const { return parameters; }

Token FunctionStmt::getToken() const { return name; }

std::vector<std::shared_ptr<Stmt>> FunctionStmt::getFunctionBody() const {
  return functionBody;
}

} // namespace arsenic
