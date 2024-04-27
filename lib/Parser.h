#pragma once

#include "macros.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream> 

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

  SymbolInfo() : memory_adress(0) {}
};

class Parser
{
public:
  Parser(std::string path)
  {
#ifdef _WIN32
    command=R"(G:\DRIL\lib\vsdev.bat)";
    filename2=R"(G:\DRIL\examples\OpenGL\bin\debug\output2.txt)";
    filename1=R"(G:\DRIL\examples\OpenGL\bin\debug\output.txt)";
#elif __APPLE__
    command = "nm -C \"" + path + "\"";
#else
    command = "readelf -Ws "+ path +R"( | awk '/Symbol table '\''\.dynsym'\''/ {flag=1; next} flag && $2 != "0000000000000000" && NR > 5 {print $4, $8} flag && /^$/ {exit}'
            )";
#endif
  }

  std::unordered_map<std::string, SymbolInfo> ExtPrsSymbolTable()
  {
#ifdef _WIN32
    symbol_map.clear();
    read_pdata = popen(command.c_str(), "r");
    if (!read_pdata) {
        std::cerr << "Error opening read_pdata for command: " << command << std::endl;
    }
    while (fgets(buffer, sizeof(buffer), read_pdata) != nullptr) {
    }
    int status = pclose(read_pdata);
    if (status == -1) {
        std::cerr << "Error closing read_pdata for command: " << command << std::endl;
    }
    // TO STORE 
    std::vector<std::string> names;
    std::unordered_map<std::string, SymbolInfo> symbol_map;

    // FILE STREAMS
    std::ifstream file1(filename1);
    std::ifstream file2(filename2);

    if (file1.is_open())
    {
        std::string line;
        bool parsing = false;
        std::regex pattern(R"(\b[^0-9\s][^\d]*[^0-9\s]\b)");

        while (std::getline(file1, line))
        {
            if (line.find("RVA") != std::string::npos)
            {
                parsing = true;
                continue;
            }
            if (line.find("Summary") != std::string::npos)
            {
                break;
            }
            if (parsing)
            {
                std::smatch matches;
                std::sregex_iterator it(line.begin(), line.end(), pattern);
                std::sregex_iterator end;
                for (; it != end; ++it)
                {
                    names.push_back(it->str());
                }
            }
        }
        file1.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename1 << std::endl;
    }
    
    if (!file2.is_open())
    {
        std::cerr << "Unable to open file: " << filename2 << std::endl;
        return symbol_map;
    }

    std::string line;
    while (std::getline(file2, line))
    {
      for (const auto &searchString : names)
      {
        if (line.find(searchString) != std::string::npos)
        {
          SymbolInfo symb_info;
          EntityType type = EntityType::FUNC;
          symb_info.type = type;

          // Check if "__nm_" + searchString is in the line
          if (line.find("__nm_" + searchString) != std::string::npos)
          {
            type = EntityType::OBJECT;
            symb_info.type = type;
          }

          symbol_map[searchString] = symb_info; // Simplified assignment
          break;
        }
      }
    }
    file2.close();
#elif __APPLE__
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
#else
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
    std::istringstream input_stream(extracted_data);
    std::string line;
    std::string str_type,str_name;
    std::istringstream line_stream;
    struct SymbolInfo symb_info;
    symbol_map.clear();
    while (std::getline(input_stream, line)) 
    {
        
        line_stream.clear();
        line_stream.str(line);
        if (line_stream >> str_type >> str_name) 
        {
            
            if(!symbol_map.count(str_name))
            {
                
                if(str_type=="FUNC")
                {
                    symb_info.type=EntityType::FUNC;
                    symbol_map[str_name]=symb_info;
                }
                else if(str_type=="OBJECT")
                {
                    symb_info.type=EntityType::OBJECT;
                    symbol_map[str_name]=symb_info;
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


// #pragma once

// #include "macros.h"
// #include <iostream>
// #include <regex>
// #include <sstream>
// #include <string>
// #include <vector>
// #include <unordered_map>
// #include <fstream> // Added this include for std::ifstream

// constexpr int BUFFER_SIZE = 100;

// enum EntityType
// {
//   OBJECT,
//   FUNC,
// };

// struct SymbolInfo
// {
//   EntityType type;
//   FPTR memory_adress;

//   SymbolInfo() : memory_adress(0) {}
// };

// class Parser
// {
// public:
//   Parser(std::string path)
//   {
// #ifdef _WIN32
//     command = R"(G:\DRIL\lib\vsdev.bat)";
//     filename2 = path + "\\output2.txt"; // Moved filename assignments here
//     filename1 = path + "\\output.txt";
// #else
//     command = "nm -C \"" + path + "\"";
// #endif
//   }

//   std::unordered_map<std::string, SymbolInfo> ExtPrsSymbolTable()
//   {
//     std::unordered_map<std::string, SymbolInfo> symbol_map;

// #ifdef _WIN32
//     // FILE STREAMS
//     std::ifstream file1(filename1);
//     std::ifstream file2(filename2);

//     if (!file1.is_open())
//     {
//       std::cerr << "Unable to open file: " << filename1 << std::endl;
//       return symbol_map;
//     }
//     // Rest of your code...
// #else
//     // Rest of your code...
// #endif

//     return symbol_map;
//   }

// private:
//   std::string filename1;
//   std::string filename2;
//   std::unordered_map<std::string, SymbolInfo> symbol_map;
//   char buffer[BUFFER_SIZE];
//   std::string extracted_data;
//   std::string command;
//   FILE *read_pdata;
// };
