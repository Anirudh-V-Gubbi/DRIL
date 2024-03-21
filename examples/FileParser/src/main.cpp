#include <iostream>
#include <Interface.h>
#include <string>

int main()
{
    DRIL interface(
    DLL_PATH "/libFileParserApplication.dylib",
    APPLICATION_PATH,
    OUTPUT_PATH);
    interface.LoadILibrary();

    interface.Execute<void>("parseFile", std::string("/Users/anirudh/ACM/DRIL/examples/FileParser/main.json"));

    return 0;
}