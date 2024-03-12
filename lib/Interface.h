#pragma once

#include "Parser.h"
#include "macros.h"
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <unordered_map>

// #############################################################################
//                           DRIL
// #############################################################################

class DRIL
{
public:
  DRIL(std::string path) : m_path(path), parser(path) {}

  void LoadLibrary() { Load(0); }
  void ReloadLibrary() { Reload(); }

  template <typename Ret, typename... Args>
  Ret Execute(const char *name, Args... args)
  {
    // Lookup the function address
    auto symbol = m_symbols.find(name);
    if (symbol != m_symbols.end())
    {
      // Cast the address to the appropriate function type and call it,
      // forwarding all arguments
      return reinterpret_cast<Ret (*)(Args...)>(symbol->second.memory_adress)(args...);
    }
    else
    {
      throw std::runtime_error(std::string("Function not found: ") + name);
    }
  }

  template <typename T>
  T *GetVar(const char *name)
  {
    auto symbol = m_symbols.find(name);
    if (symbol != m_symbols.end())
    {
      return reinterpret_cast<T *>(symbol->second.memory_adress);
    }
    else
    {
      // We didn't find the variable. Throws an error
      throw std::runtime_error(std::string("Variable not found") + name);
    }
  }

protected:
  //   // Should return a reference to an array of C-strings of size NumSymbols
  //   // Used when loading or reloading the library to lookup the address of
  //   // all exported symbols
  //   virtual std::array<const char *, NumSymbols> &GetSymbolNames() const = 0;
  // }

private:
  std::string m_path;
  Parser parser;
  void *m_libHandle;
  std::unordered_map<std::string, SymbolInfo> m_symbols;

  bool copyFile(const char *sourcePath, const char *destinationPath)
  {
    // Open source file for reading in binary mode
    std::ifstream sourceFile(sourcePath, std::ios::binary);
    if (!sourceFile)
    {
      std::cerr << "Error opening source file: " << sourcePath << std::endl;
      return false;
    }

    // Open destination file for writing in binary mode
    std::ofstream destinationFile(destinationPath, std::ios::binary);
    if (!destinationFile)
    {
      std::cerr << "Error opening destination file: " << destinationPath
                << std::endl;
      return false;
    }

    // Copy content from source to destination
    destinationFile << sourceFile.rdbuf();

    if (destinationFile.fail())
    {
      std::cerr << "Error copying file." << std::endl;
      return false;
    }
    sourceFile.close();
    destinationFile.close();

    return true;
  }

  void Load(bool isWindowsReload)
  {
    if (isWindowsReload == 0)
    {
      m_libHandle = dril_load(GetPath());
    }
    else
    {
      std::string path = GetPath();
      size_t pos = path.rfind('.');
      if (pos != std::string::npos)
      {
        path.insert(pos, "_Copy");
      }
      const char *CopyPath = path.c_str();
      copyFile(GetPath(), CopyPath);
      m_libHandle = dril_load(CopyPath);
    }
    LoadSymbols();
  }

  void Reload()
  {
    OS_HANDLE handle = static_cast<OS_HANDLE>(m_libHandle);
    dril_close(handle);
    m_symbols.clear();
    dril_reload();
  }

  void LoadSymbols()
  {
    m_symbols = parser.ExtPrsSymbolTable();

    OS_HANDLE handle = static_cast<OS_HANDLE>(m_libHandle);
    for (auto &symbol : m_symbols)
    {
      m_symbols[symbol.first].memory_adress =
          lib_find(handle, symbol.first.c_str());
    }
  }
};