#include <Parse/Interpret.h>
#include <RoutineInterface/ArsenicClass.h>
#include <RoutineInterface/ArsenicInstance.h>

namespace arsenic {

ArsenicClass::ArsenicClass(
    std::string name,
    std::unordered_map<std::string, std::shared_ptr<ArsenicFunction>> Methods)
    : name(name), Methods(Methods) {}

std::string ArsenicClass::toString() { return name; }

std::any ArsenicClass::call(Interpreter interpreter,
                            std::vector<std::any> arguments) {

  std::shared_ptr<ArsenicInstance> instance =
      std::make_shared<ArsenicInstance>(*this);

  return instance;
}

int ArsenicClass::arity() { return 0; }

std::any ArsenicClass::findMethod(std::string Name) {

  if (auto search = Methods.find(Name); search != Methods.end())
    return search->second;

  return nullptr;
}

} // namespace arsenic
