#ifndef __ARSENICCLASS__
#define __ARSENICCLASS__

#include <RoutineInterface/ArsenicCallable.h>
#include <RoutineInterface/ArsenicFunction.h>
#include <memory>
#include <string>

namespace arsenic {

class ArsenicClass : public ArsenicCallable {

  std::string name;
  std::shared_ptr<ArsenicClass> SuperClass;
  std::unordered_map<std::string, std::shared_ptr<ArsenicFunction>> Methods;

public:
  ArsenicClass(std::string name, std::shared_ptr<ArsenicClass> SuperClass,
               std::unordered_map<std::string, std::shared_ptr<ArsenicFunction>>
                   methods);
  std::string toString();
  std::any call(Interpreter interpreter,
                std::vector<std::any> arguments) override;
  int arity() override;
  std::any findMethod(std::string Name);
};

} // namespace arsenic

#endif // !__ARSENICCLASS__
