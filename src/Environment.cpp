#include <Environment.h>
#include <Error/arsenic_runtime_error>

namespace arsenic {

void Environment::define(std::string name, std::any value) {
  values.insert({name, value});
}

std::any Environment::get(Token name) {
  if(auto search = values.find(name.getLexeme()); search != values.end())
    return search->second;

  throw new RuntimeError(name, "Undefined variable '" + name.getLexeme() + "'.");
}

void Environment::assign(Token name, std::any value) {
  if (auto search = values.find(name.getLexeme()); search != values.end()) {
    search->second = value;
    return;
  }

  throw new RuntimeError(name, "Undefined variable '" + name.getLexeme() + "'.");
}

}
