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
#include "fileWatcher.h"

// #############################################################################
//                           DRIL
// #############################################################################

class DRIL
{

public:
  DRIL(std::string pathOfLib, std::string pathToWatch, std::string pathToBuild) : m_path(pathOfLib), parser(pathOfLib)
  {
    filewatcher.watchOverDirAndSubDir(pathToWatch, [pathToBuild, this]() {
      function(pathToBuild);
    });
  }

  void LoadILibrary() { Load(0); }
  void ReloadLibrary() { Reload(); }

  bool Exists(const char* name) const
  {
    for (auto symbol : m_symbols)
    {
      if(symbol.first == name && symbol.second.memory_adress != 0) return true;
    }

    return false;
  }

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

private:
  std::string m_path;
  Parser parser;
  void *m_libHandle;
  std::unordered_map<std::string, SymbolInfo> m_symbols;
  CustomFileWatcher filewatcher;

  void function(std::string pathToBuild)
  {
    OS_HANDLE handle = static_cast<OS_HANDLE>(m_libHandle);
    dril_close(handle);

    std::string command = "cd \"" + pathToBuild + "\" && make --file=OpenGLApplication.make";
    std::system(command.c_str());
    ReloadLibrary();
  }
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
      m_libHandle = dril_load(m_path.c_str());
    }
    else
    {
      std::string path = m_path;
      size_t pos = path.rfind('.'); // To find the last .
      if (pos != std::string::npos)
      {
        path.insert(pos, "_Copy");
      }
      const char *CopyPath = path.c_str();
      m_libHandle = dril_load(CopyPath);
    }
    LoadSymbols();
    std::cout << "Library " << m_libHandle << " loaded\n";
  }

  void Reload()
  {
    OS_HANDLE handle = static_cast<OS_HANDLE>(m_libHandle);
    dril_close(handle);
    m_symbols.clear();

#ifdef _WIN32
    std::string path = m_path;
    size_t pos = path.rfind('.');
    if (pos != std::string::npos)
    {
      path.insert(pos, "_Copy");
    }
    const char *CopyPath = path.c_str();

    copyFile(path.c_str(), CopyPath);
#endif
    
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