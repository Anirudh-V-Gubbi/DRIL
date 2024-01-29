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

class ReplexModule;
class Parser
{
    private:
            char buffer[BUFFER_SIZE];
            std::string extracted_data;
            std::string command;
            std::unordered_map<std::string,struct SymbolInfo> symbol_map_export;
            FILE* read_pdata;
            friend class ReplexModule;
    public:
        Parser(std::string lib_path)
        {
           command = 
                "readelf -Ws "+ lib_path +R"( | awk '/Symbol table '\''\.dynsym'\''/ {flag=1; next} flag && $2 != "0000000000000000" && NR > 5 {print $4, $8} flag && /^$/ {exit}'
            )";
        }

        
        void ExtPrsSymbolTable()
        {
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
            symbol_map_export.clear();
            while (std::getline(input_stream, line)) 
            {
                
                line_stream.clear();
                line_stream.str(line);
                if (line_stream >> str_type >> str_name) 
                {
                    if(!symbol_map_export.count(str_name))
                    {
                        if(str_type=="FUNC")
                        {
                            symb_info.type=EntityType::FUNC;
                            symbol_map_export[str_name]=symb_info;
                        }
                        else if(str_type=="OBJECT")
                        {
                            symb_info.type=EntityType::OBJECT;
                            symbol_map_export[str_name]=symb_info;
                        }
                        else
                        {

                        }
                    }
                
                
                } 
            }
            extracted_data.clear();
        }
        std::unordered_map<std::string,struct SymbolInfo>& ReturnSymbolMap()
        {
            return symbol_map_export;
        }
        

        
            


};

class RePlexModule
{
public:
  
  RePlexModule(){}
   
   void LoadLibrary(std::string lib_path) 
   { 
        
       /*if (lib_path_set.find(lib_path) == lib_path_set.end()) 
        {
                lib_path_set.insert(lib_path);
                path = lib_path;
                
        }
        else
        {
            std::cerr<<"Cannot use two objects for the same path"<<std::endl;
        }*/
            
            GetInstance().Load(); 
            
            
    }

   
   void ReloadLibrary() { GetInstance().Reload(); }

   template <typename Ret, typename... Args> 
    Ret GetEntity(std::string name, Args... args)
    {
    
        auto symbol = prs->ReturnSymbolMap().find(name);
        if (symbol != prs->ReturnSymbolMap().end())
        {

            if(symbol->second.type==EntityType::FUNC)
            {
                return reinterpret_cast<Ret(*)(Args...)>(symbol->second.memory_adress)(args...);
            }
            else
            {
                return static_cast<Ret>(symbol->second.memory_adress);
            }
        }
        else
        {
        throw std::runtime_error(std::string("Entity not found: ") + name);
        }
    }
    
   
    
private:
    RePlexModule& GetInstance()
    {
        if(!instance)
        {
            instance = new RePlexModule();
        }
        
        return *instance;
    }

    void Load()
    {
        std::cout<<"passed";
        m_libHandle = dlopen(path.c_str(), RTLD_LAZY);
        prs=new Parser(path);
        GetSymbols();
        
    }

    void Reload()
    {
        dlclose(m_libHandle);
        Load();
        GetSymbols();
    }
     void GetSymbols()
    {
        struct SymbolInfo symb_info;
        for(auto symbol :prs->ReturnSymbolMap())
        {
            prs->ReturnSymbolMap()[symbol.first].memory_adress=dlsym(m_libHandle,symbol.first.c_str());
         
        }

    }
   
    

  void* m_libHandle;
  std::string path;
 
  static std::unordered_set<std::string> lib_path_set;
  RePlexModule* instance;
  Parser* prs;
 
     
    
};
int main()
{
    RePlexModule r;
    r.LoadLibrary("/home/adv/DRILtesting/libtest.so");
    r.GetEntity<int*>("bar");
}





