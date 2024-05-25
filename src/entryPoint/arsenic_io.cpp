#include <Common/arsenic_io>
#include <Error/arsenic_error>
#include <Lex/Lex.h>
#include <Parse/Interpret.h>
#include <Parse/Parse.h>
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <sys/stat.h>
#include <sys/types.h>
#include <system_error>
#include <unistd.h>
#include <vector>

namespace arsenic {

void run(const char *MemoryBuffer);

void runFile(const char *FileName) {

  InitializeErrorReporting();

  int FD = open(FileName, O_RDONLY);
  // open(2) returns a -1 if the file could not be opened.
  // In this case, print an error and return.
  if (FD < 0) {
    std::error_code Err(errno, std::generic_category());
    std::cerr << "[ERROR] Could not open file \"" << FileName
              << "\": " << Err.message() << std::endl;
    return;
  }

  struct stat Stat;

  if (fstat(FD, &Stat) < 0) {
    std::error_code Err(errno, std::generic_category());
    std::cerr << "[Error] Could not aquire Information "
              << "on " << FileName << ": " << Err.message() << std::endl;
  }

  off64_t FileSize = Stat.st_size;

  char *MemoryBuffer =
      static_cast<char *>(operator new(FileSize + 1, std::nothrow));
  MemoryBuffer[FileSize] = 0;

  const ssize_t ChunkSize = 16;
  ssize_t Offset = 0;
  ssize_t ReadBytes = 0;
  do {
    ReadBytes = read(FD, MemoryBuffer + Offset, ChunkSize);
    if (ReadBytes < 0) {
      std::error_code Err(errno, std::generic_category());
      std::cerr << "[ERROR] Could not read from file "
                   "descriptor \""
                << FD << "\": " << Err.message() << std::endl;
      delete MemoryBuffer;
      return;
    }
    Offset += ReadBytes;
  } while (ReadBytes != 0);

  run(MemoryBuffer);

  if (arsenicError.hadError()) {
    exit(1);
  }

  delete MemoryBuffer;

  if (close(FD) < 0) {
    std::error_code Err(errno, std::generic_category());
    std::cerr << "[ERROR] Could not close file "
              << "descriptor \"" << FD << "\":" << Err.message() << std::endl;
    return;
  }
}

void runPrompt() {

  // run(LineBuffer);
  // Error.clear();
}

void run(const char *MemoryBuffer) {

  Scanner scanner(MemoryBuffer);

  std::vector<Token> tokens = scanner.scanTokens();
  std::unique_ptr<Parser> parser = std::make_unique<Parser>(tokens);
  //  std::unique_ptr<Expr> expression = parser->parse();
  //
  Interpreter *interpreter = new Interpreter();

  std::vector<std::unique_ptr<Stmt>> statements = parser->parse();

  //  interpreter->interpret(expression);
  interpreter->interpret(statements);
}

} // namespace arsenic
