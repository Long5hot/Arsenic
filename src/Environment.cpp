#include <Environment.h>
#include <Error/arsenic_runtime_error>

namespace arsenic {

Environment::Environment() : enclosing(nullptr) {}

Environment::Environment(std::shared_ptr<Environment> enclosing)
    : enclosing(enclosing) {}

void Environment::define(std::string name, std::any value) {
  values.insert({name, value});
}

std::any Environment::get(Token name) {
  if (auto search = values.find(name.getLexeme()); search != values.end())
    return search->second;

  if (enclosing != nullptr)
    return enclosing.get()->get(name);

  throw new RuntimeError(name,
                         "Undefined variable '" + name.getLexeme() + "'.");
}

void Environment::assign(Token name, std::any value) {
  if (auto search = values.find(name.getLexeme()); search != values.end()) {
    search->second = value;
    return;
  }

  if (enclosing != nullptr) {
    enclosing->assign(name, value);
    return;
  }

  throw new RuntimeError(name,
                         "Undefined variable '" + name.getLexeme() + "'.");
}

} // namespace arsenic
