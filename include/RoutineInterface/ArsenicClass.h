#ifndef __ARSENICCLASS__
#define __ARSENICCLASS__

#include <RoutineInterface/ArsenicCallable.h>
#include <string>

namespace arsenic {

class ArsenicClass : public ArsenicCallable {

  std::string name;

public:
  ArsenicClass(std::string name);
  std::string toString();
  std::any call(Interpreter interpreter,
                std::vector<std::any> arguments) override;
  int arity() override;
};

} // namespace arsenic

#endif // !__ARSENICCLASS__
