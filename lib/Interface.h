#pragma once
#include <iostream>
#include <array>
#include <unordered_map>
#include <string>
#include <stdexcept> 
#include <stdio.h>
#include <fstream>
#include "macros.h"



// #############################################################################
//                           CopyFile
// #############################################################################

bool copyFile(const char* sourcePath, const char* destinationPath) {
    // Open source file for reading in binary mode
    std::ifstream sourceFile(sourcePath, std::ios::binary);
    if (!sourceFile) {
        std::cerr << "Error opening source file: " << sourcePath << std::endl;
        return false;
    }

    // Open destination file for writing in binary mode
    std::ofstream destinationFile(destinationPath, std::ios::binary);
    if (!destinationFile) {
        std::cerr << "Error opening destination file: " << destinationPath << std::endl;
        return false;
    }

    // Copy content from source to destination
    destinationFile << sourceFile.rdbuf();

    if (destinationFile.fail()) {
        std::cerr << "Error copying file." << std::endl;
        return false;
    }
    sourceFile.close();
    destinationFile.close();

    return true;
}
// #############################################################################
//                           DRILModule
// #############################################################################

template <typename E, size_t NumSymbols>
class DRILModule
{
public:
  static void Loadlibrary() { GetInstance().Load(1); }
  static void Reloadlibrary() { GetInstance().Reload(); }
protected:
  static E& GetInstance()
  {
    static E instance;
    return instance;
  }

  // Should return the path to the library on disk
  virtual const char* GetPath() const = 0;

  // Should return a reference to an array of C-strings of size NumSymbols
  // Used when loading or reloading the library to lookup the address of
  // all exported symbols
  virtual std::array<const char*, NumSymbols>& GetSymbolNames() const = 0;

  template <typename Ret, typename... Args>
  Ret Execute(const char* name, Args... args)
  {
    // Lookup the function address
    auto symbol = m_symbols.find(name);
    if (symbol != m_symbols.end())
    {
      // Cast the address to the appropriate function type and call it,
      // forwarding all arguments
      return reinterpret_cast<Ret(*)(Args...)>(symbol->second)(args...);
    }
    else
    {
      throw std::runtime_error(std::string("Function not found: ") + name);
    }
  }

  template <typename T>
  T* GetVar(const char* name)
  {
    auto symbol = m_symbols.find(name);
    if (symbol != m_symbols.end())
    {
      return reinterpret_cast<T*>(symbol->second);
    }
    else
    {
      // We didn't find the variable. Return an empty pointer
      return nullptr;
    }
  }

private:
    
  void Load(bool ch)
  {
    if(ch==1){
    m_libHandle = dril_load(GetPath()); //Important LoadLibraryA
    }
    else{
      std::string path=GetPath();
      size_t pos = path.find('.');
      if (pos != std::string::npos) {
          path.insert(pos, "_copy");
      }
      const char* CopyPath=path.c_str();
      m_libHandle = dril_load(CopyPath);
    }
    LoadSymbols();

  }

  void Reload()
  {
    OS_HANDLE handle = static_cast<OS_HANDLE>(m_libHandle);
    dril_close(handle);
    m_symbols.clear();
    std::string path=GetPath();
    size_t pos = path.find('.');
    if (pos != std::string::npos) {
        path.insert(pos, "_Copy");
    }
    const char* CopyPath=path.c_str();
    drilcp;
    dril_reload();

  }

  void LoadSymbols()
  {
    for (const char* symbol : GetSymbolNames())
    {
      OS_HANDLE handle = static_cast<OS_HANDLE>(m_libHandle);

      m_symbols[symbol] = lib_find(handle, symbol);
    }
  }
  void* m_libHandle;
  std::unordered_map<std::string, fptr> m_symbols;
};