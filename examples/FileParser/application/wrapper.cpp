#include "wrapper.h"

void parseFile(std::string file_name) {
  try {
    std::cout << file_name << std::endl;
    std::ifstream fin(file_name);
    std::stringstream ss;
    ss << fin.rdbuf();
    const std::string &data = ss.str();

    yazi::json::Json json;
    json.parse(data);

    const string &name = json["name"];
    std::cout << name << '\n';
    int type = json["age"];
    std::cout << type << '\n';
    const string &college = json["college"];
    std::cout << college << '\n';
    const string &area = json["area"][0];
    std::cout << area << '\n';
    json.clear();
  } catch (std::exception &e) {
    std::cout << "Error : " << e.what() << std::endl;
  }
}