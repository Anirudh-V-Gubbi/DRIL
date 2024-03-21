#ifndef WRAPPER_H
#define WRAPPER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <json/Json.h>

extern "C" {
    void parseFile(std::string file_name);
}

#endif