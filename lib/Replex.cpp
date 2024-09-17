#include <iostream>
#include<sstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <stdexcept>
#include <dlfcn.h>

constexpr int BUFFER_SIZE=100;

enum EntityType
{
    OBJECT,
    FUNC,
};


struct SymbolInfo
{
    EntityType type;
    void* memory_adress;
};


class RePlexModule
{
public:
     
  
  RePlexModule(){}
   
   void LoadLibrary(const std::string& lib_path) 
   { 
        path=lib_path;
       RePlexModule::Load();           
            
    }

   
   void ReloadLibrary() { RePlexModule::Reload(); }

   template <typename Ret, typename... Args> 
    Ret GetEntity(std::string name, Args... args)
    {
        
        auto symbol = symbol_map.find(name);
        if (symbol != symbol_map.end())
        {

            if(symbol->second.type==EntityType::FUNC)
            {
                return reinterpret_cast<Ret(*)(Args...)>(symbol->second.memory_adress)(args...);
            }
            else
            {
                return *static_cast<Ret*>(symbol->second.memory_adress);
            }
        }
        else
        {
        throw std::runtime_error(std::string("Entity not found: ") + name);
        }
    }
    template <typename Ret> 
    Ret GetEntity(std::string name)
    {
        
        auto symbol = symbol_map.find(name);
        if (symbol != symbol_map.end())
        {

            if(symbol->second.type==EntityType::FUNC)
            {
                return *reinterpret_cast<Ret(*)>(symbol->second.memory_adress);     }
            else
            {
                return *static_cast<Ret*>(symbol->second.memory_adress);
            }
        }
        else
        {
        throw std::runtime_error(std::string("Entity not found: ") + name);
        }
    }
    
private:  
   
    void Load()
    {
        
        m_libHandle = dlopen(path.c_str(), RTLD_NOW);
        RePlexModule::ExtPrsSymbolTable();
        GetSymbols();
        
    }

    void Reload()
    {
        dlclose(m_libHandle);
        Load();
        RePlexModule::ExtPrsSymbolTable();
        GetSymbols();
    }
     void GetSymbols()
    {
        
        struct SymbolInfo symb_info;
        for(auto symbol :symbol_map)
        {
    
            symbol_map[symbol.first].memory_adress=dlsym(m_libHandle,symbol.first.c_str());
         
        }

    }

    void ExtPrsSymbolTable()
        {
             command = 
                "readelf -Ws "+ path +R"( | awk '/Symbol table '\''\.dynsym'\''/ {flag=1; next} flag && $2 != "0000000000000000" && NR > 5 {print $4, $8} flag && /^$/ {exit}'
            )";
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
            extracted_data.clear();
        }
   
    

  void* m_libHandle;
  std::string path;
 
  RePlexModule* instance;
  
  friend class Parser;
  friend class ReplexModule;
  char buffer[BUFFER_SIZE];
  std::string extracted_data;
  std::string command;
  std::unordered_map<std::string,struct SymbolInfo> symbol_map;
  FILE* read_pdata;
 
     
    
};
int main()
{
    RePlexModule r;
    r.LoadLibrary("/home/adv/DRILtesting/libtest.so");
    std::cout<<r.GetEntity<int>("bar")<<'\n';
    std::cout << "Make some changes, recompile, and press enter." << std::flush;
    while(std::cin.get() != '\n') {}
    r.ReloadLibrary();
    std::cout<<r.GetEntity<int>("bar");
    
}




