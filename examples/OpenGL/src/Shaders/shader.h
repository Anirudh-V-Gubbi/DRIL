#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Logger/logger.h"
#define RESOURCE_PATH R"(G:\DRIL\examples\OpenGL\resources\)"

class Shader {
public:
    unsigned int ID;

    Shader(std::string vertexPath, std::string fragmentPath) {
        // create a struct to hold shader code, deallocated in compileShaders()
        // std::cout << "compiled1 ";
        shaderCode = new ShaderCodes();
        // std::cout << "compiled2 " << getShaderFullPath(vertexPath);

        m_shaderName = vertexPath.substr(0, vertexPath.find('_'));
        // std::cout << "compiled3 " << getShaderFullPath(vertexPath);
        this->readFiles(getShaderFullPath(vertexPath), getShaderFullPath(fragmentPath));
        // std::cout << "compiled4 " << getShaderFullPath(vertexPath);
        this->compileShaders();
        std::cout << "compiled5 " << getShaderFullPath(vertexPath);
    }

    ~Shader() {
    }
    
    void Use() const {
        glUseProgram(this->ID);
    };
    
    void SetMatrix4f(const char* name, glm::mat4 matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    void SetVector2f(const char* name, glm::vec2 vector) const {
        glUniform2fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(vector));
    }
    
    void SetVector3f(const char* name, glm::vec3 vector) const {
        glUniform3fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(vector));
    }
    
    void SetInteger1i(const char* name, int unit) const {
        glUniform1i(glGetUniformLocation(this->ID, name), unit);
    }
    
private:

    struct ShaderCodes {
        std::string vertexCode;
        std::string fragmentCode;
    } *shaderCode;

    std::string m_shaderName;
    
    void readFiles(std::string vertexPath, std::string fragPath){
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        
        try {
            vShaderFile.open(vertexPath.c_str());
            fShaderFile.open(fragPath.c_str());
            
            std::stringstream sCodeBuffer, fCodeBuffer;
            
            sCodeBuffer << vShaderFile.rdbuf();
            fCodeBuffer << fShaderFile.rdbuf();
            
            this->shaderCode->vertexCode = sCodeBuffer.str();
            this->shaderCode->fragmentCode = fCodeBuffer.str();
            
            vShaderFile.close();
            fShaderFile.close();
        }
        catch(const std::exception& e) {
            std::cout << "Failed to read shader files for" << m_shaderName;
            // Logger::GetInstance()->error("Failed to read shader files for", m_shaderName);
        }
    }
    
    void compileShaders() {
        std::cout << "Compiling" << shaderCode << std::endl;
        GLuint vertexID, fragID;
        
        const char* vertexCode = shaderCode->vertexCode.c_str();
        const char* fragmentCode = shaderCode->fragmentCode.c_str();

        
        //create vertex shader
        vertexID = glCreateShader(GL_VERTEX_SHADER);
        std::cout << "Compiling" << vertexCode << std::endl;
        glShaderSource(vertexID, 1, &vertexCode, NULL);
        glCompileShader(vertexID);

        std::cout << "Created" << std::endl;
        
        int success;
        char infolog[512];
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);

        std::cout << "IV" << std::endl;
        
        //vertex shader compilation failed
        if(!success) {
            glGetShaderInfoLog(vertexID, 512, NULL, infolog);
            std::cout << "Failed to compile" << m_shaderName << "vertex shader -\n" << infolog;
            // Logger::GetInstance()->error("Failed to compile", m_shaderName, "vertex shader -\n", infolog);
        }
        
        //create fragment shader
        fragID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragID, 1, &fragmentCode, NULL);
        glCompileShader(fragID);
        
        glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);
        
        //fragment shader compilation failed
        if(!success) {
            glGetShaderInfoLog(fragID, 512, NULL, infolog);
            std::cout << "Failed to compile" << m_shaderName << "fragment shader -\n" << infolog;
            // Logger::GetInstance()->error("Failed to compile", m_shaderName, "fragment shader -\n", infolog);
        }
        
        //create and link shader program
        this->ID = glCreateProgram();
        glAttachShader(this->ID, vertexID);
        glAttachShader(this->ID, fragID);
        glLinkProgram(this->ID);
        
        glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
        
        //shader program linking fails
        if(!success) {
            glGetProgramInfoLog(this->ID, 512, NULL, infolog);
            std::cout << "Failed to link" << m_shaderName << "shader program -\n" << infolog << std::endl;
            // Logger::GetInstance()->error("Failed to link", m_shaderName, "shader program -\n", infolog);
        }
        
        //delete shader objects
        glDeleteShader(vertexID);
        glDeleteShader(fragID);
        
        delete shaderCode;
    }

    std::string getShaderFullPath(std::string path) {
        return std::string(RESOURCE_PATH) + "shaders\\" + path;
    }
};

#endif
