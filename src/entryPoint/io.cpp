#include <unistd.h>
#include <fcntl.h>
#include <system_error>
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include <iostream>


namespace arsenic {

void runFile(const char * FileName) {


  int FD = open(FileName, O_RDONLY);
    // open(2) returns a -1 if the file could not be opened.
  // In this case, print an error and return.
  if (FD < 0) {
    std::error_code Err(errno, std::generic_category());
    std::cerr << "[ERROR] Could not open file \""
              << FileName << "\": " << Err.message()
              << std::endl;
    return;
  }

  struct stat Stat;

  if(fstat(FD, &Stat) < 0) {
    std::error_code Err(errno, std::generic_category());
    std::cerr << "[Error] Could not aquire Information "
              << "on " << FileName << ": " << Err.message() << std::endl; 
  }

  off64_t FileSize = Stat.st_size;

  char *MemoryBuffer = static_cast<char *>(operator new(
      FileSize + 1, std::nothrow));
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
                << FD << "\": " << Err.message()
                << std::endl;
      delete MemoryBuffer;
      return;
    }
    Offset += ReadBytes;
  } while (ReadBytes != 0);

  char *Ptr = MemoryBuffer;
  while (*Ptr != '\0') {
    std::cout << *Ptr;
    ++Ptr;
  }
  std::cout << "\"" << std::endl;

  delete MemoryBuffer;

  if (close(FD) < 0) {
    std::error_code Err(errno, std::generic_category());
    std::cerr << "[ERROR] Could not close file "
              << "descriptor \"" << FD << "\":"
              << Err.message() << std::endl;
    return;
  }

  return;
  //run(Buffer);.
}

void runPrompt() {
  
}
  
}
