#include "Interface.h"

int main()
{

  DRIL r("dllPath");
  r.LoadILibrary();
  std::cout << "Make some changes, recompile, and press enter." << std::flush;
  while (std::cin.get() != '\n')
  {
  }
  return 0;
}
