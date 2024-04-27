#include <iostream>
#include <Interface.h>
#include <string>

int main()
{
    DRIL interface(DLL_PATH "/libFileParserApplication.so");
    interface.Initialise(APPLICATION_PATH, OUTPUT_PATH, [&interface]() {
        interface.Execute<void>("parseFile", std::string("/home/hundi/DRIL/examples/FileParser/main.json"));
    });
    interface.LoadILibrary();

    while(std::cin.get() != '\n') {

    }

    return 0;
}