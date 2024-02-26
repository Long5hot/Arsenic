#include <common/arsenic_error>
#include <common/arsenic_io>
#include <iostream>
#include <stdlib.h>

namespace arsenic {
  

errorReporting arsenicError;

int arsenic_main(int argc, char *argv[]) {

  if (argc > 2) {
    std::cout << "Usage: arsenic [filename]" << std::endl;
    exit(EXIT_FAILURE);
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
  return 0;
}

}

int main (int argc, char *argv[]) {
  arsenic::arsenic_main(argc, argv);
  return 0;
}
