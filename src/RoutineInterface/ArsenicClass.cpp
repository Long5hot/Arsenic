#include <Parse/Interpret.h>
#include <RoutineInterface/ArsenicClass.h>
#include <RoutineInterface/ArsenicInstance.h>

namespace arsenic {

ArsenicClass::ArsenicClass(std::string name) : name(name) {}

std::string ArsenicClass::toString() { return name; }

std::any ArsenicClass::call(Interpreter interpreter,
                            std::vector<std::any> arguments) {

  std::shared_ptr<ArsenicInstance> instance =
      std::make_shared<ArsenicInstance>(*this);

  return instance;
}

int ArsenicClass::arity() { return 0; }

} // namespace arsenic
