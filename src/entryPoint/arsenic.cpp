#include <Common/arsenic_io>
#include <Error/arsenic_error>
#include <iostream>
#include <stdlib.h>

namespace arsenic {

errorReporting arsenicError;

int arsenic_main(int argc, char *argv[]) {

  if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
  return 0;
}

} // namespace arsenic

int main(int argc, char *argv[]) {
  if (argc > 2) {
    std::cout << "Usage: arsenic [filename]" << std::endl;
    exit(EXIT_FAILURE);
  }
  arsenic::arsenic_main(argc, argv);
  return 0;
}
