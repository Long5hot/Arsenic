#ifndef __ARSENICCALLABLE__
#define __ARSENICCALLABLE__

#include <Parse/Interpret.h>
#include <any>

namespace arsenic {

class Interpreter;

class ArsenicCallable {

public:
  virtual std::any call(Interpreter interpreter,
                        std::vector<std::any> arguments) = 0;
  virtual int arity() = 0;
};

} // namespace arsenic

#endif // !__ARSENICCALLABLE__
