#ifndef __ARSENICFUNCTION__
#define __ARSENICFUNCTION__

#include <Parse/Stmt/FunctionStmt.h>
#include <RoutineInterface/ArsenicCallable.h>
#include <any>
#include <memory>
#include <string>
#include <vector>

namespace arsenic {

class ArsenicFunction : public ArsenicCallable {

  FunctionStmt declaration;
  std::shared_ptr<Environment> closure;
public:
  ArsenicFunction(FunctionStmt declaration, std::shared_ptr<Environment> closure);
  std::any call(Interpreter interpreter,
                std::vector<std::any> arguments) override;
  int arity() override;
  std::string toString();
};

} // namespace arsenic

#endif // !__ARSENICFUNCTION__
