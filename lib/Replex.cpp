#include "Interface.h"

int main() {
  DRIL r("/path");

  r.LoadLibrary();
  std::cout << "Make some changes, recompile, and press enter." << std::flush;
  while (std::cin.get() != '\n') {
  }
  r.ReloadLibrary();
}
