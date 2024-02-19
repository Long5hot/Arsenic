#include <iostream>
#include <stdlib.h>
#include <arsenic_io>

int main(int argc, char *argv[]) {

  if(argc > 2) {
    std::cout << "Usage: arsenic [filename]" << std::endl;
    exit(EXIT_FAILURE);
  } else if (argc == 2) {
    arsenic::runFile(argv[1]);
  } else {
    arsenic::runPrompt();
  }
}
