#include "application.h"

#include <Entities/entity.h>
#include <Shaders/shader_manager.h>
#include <Textures/texture.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

float x = 0.0002f;
float y = 0.0003f;
std::string image = "img.jpg";

extern "C" {
    float GetX() {
        return x;
    }
     float GetY() {
        return y;
    }

    std::string GetTex() {
        return image;
    }
}
