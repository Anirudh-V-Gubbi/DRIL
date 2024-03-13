#pragma once

#include "macros.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

constexpr int BUFFER_SIZE = 100;

enum EntityType
{
  OBJECT,
  FUNC,
};

struct SymbolInfo
{
  EntityType type;
  FPTR memory_adress;
};

class Parser
{
public:
  Parser(std::string path)
  {
#ifdef _WIN32
  command = "";
#else
    command = "nm -C \"" + path + "\"";
#endif
    std::cout << command << " \n";
  }

  std::unordered_map<std::string, SymbolInfo> ExtPrsSymbolTable() {
    read_pdata = popen(command.c_str(), "r");
    if (!read_pdata)
    {
      throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer, BUFFER_SIZE, read_pdata) != nullptr)
    {
      extracted_data += buffer;
    }
    if (pclose(read_pdata) == -1)
    {
      std::cerr << "pclose() failed!" << std::endl;
    }

    symbol_map.clear();


    std::regex pattern("[0-9a-fA-F]+ (\\w+) (\\w+)");
    std::sregex_iterator iterator(extracted_data.begin(), extracted_data.end(),
                                  pattern);
    std::sregex_iterator end;

    std::smatch match;
    std::string type, name;
    SymbolInfo symb_info;
    while (iterator != end)
    {
      match = *iterator;
      type = match[1];
      name = match[2];

      if (type == "T")
      {
        symb_info.type = EntityType::FUNC;
        symbol_map[name] = symb_info;
      }
      else if (type == "D")
      {
        symb_info.type = EntityType::OBJECT;
        symbol_map[name] = symb_info;
      }

      ++iterator;
    }

    std::istringstream input_stream(extracted_data);
    std::string line;
    std::string str_type, str_name;
    std::istringstream line_stream;

    while (std::getline(input_stream, line))
    {
      line_stream.clear();
      line_stream.str(line);

      if (line_stream >> str_type >> str_name)
      {

        if (!symbol_map.count(str_name))
        {

          if (str_type == "FUNC")
          {
            symb_info.type = EntityType::FUNC;
            symbol_map[str_name] = symb_info;
          }
          else if (str_type == "OBJECT")
          {
            symb_info.type = EntityType::OBJECT;
            symbol_map[str_name] = symb_info;
          }
          else
          {
          }
        }
      }
    }
#endif

    extracted_data.clear();

    return symbol_map;
  }

private:
  std::string filename1;
  std::string filename2;
  std::unordered_map<std::string, SymbolInfo> symbol_map;
  char buffer[BUFFER_SIZE];
  std::string extracted_data;
  std::string command;
  FILE *read_pdata;
};