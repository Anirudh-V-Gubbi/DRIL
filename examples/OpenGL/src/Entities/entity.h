#ifndef ENTITY_H
#define ENTITY_H

#include <glad/glad.h>
#include "..\dep\glm\glm\vec2.hpp"
#include "..\dep\glm\glm\vec3.hpp"
#include "..\dep\glm\glm\mat4x4.hpp"
//#include <glm/vec2.hpp>
//#include <glm/vec3.hpp>
//#include <glm/mat4x4.hpp>
#include "../Shaders/shader.h"
#include "../Textures/texture.h"
#include "../Logger/logger.h"

#include <iostream>
#include <memory>

class Entity {
public:

    Entity(Shader shader, Texture texture, glm::vec3 position) : m_shader{shader}, m_texture{texture}, m_position{position} {
        std::cout << "Entity" << std::endl
     ;   setup();
        std::cout << " DOne " << std::endl;
    }
    
    ~Entity() { }
    
    glm::vec3 GetPosition() const {
        return m_position;
    }
    
    GLuint GetTexID() const {
        return m_texture.ID;
    }

    Entity* GetInstance() const {
        return (Entity*)this;
    }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const {
        m_shader.Use();
        glBindVertexArray(m_VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        model = glm::scale(model, glm::vec3(m_texture.GetTexWidth(), m_texture.GetTexHeight(), 1));
        m_shader.SetMatrix4f("model", model);
        m_shader.SetMatrix4f("view", view);
        m_shader.SetMatrix4f("projection", projection);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    
    const char* GetName() const {
        return "Rect";
    }
    
    inline static unsigned int m_VBO = 0, m_VAO = 0, m_EBO = 0;
protected:
    Shader m_shader;
    Texture m_texture;
    glm::vec3 m_position;

private:
    void setup() {
        float vertices[] = {
             // positions // texture coords
             1.0f, 1.0f,  1.0f, 1.0f,   // top right
             1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
             0.0f, 0.0f,  0.0f, 0.0f,   // bottom left
             0.0f, 1.0f,  0.0f, 1.0f    // top left
        };
        unsigned int indices[] = {
            0, 3, 2,  // first Triangle
            0, 2, 1   // second Triangle
        };
        
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

#endif
