#include "application.h"

#include <Entities/entity.h>
#include <Shaders/shader_manager.h>
#include <Textures/texture.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

float x = 0.00002f;
float y = 0.00001f;
float r = 1.0f;
float g = 1.0f;
float b = 0.0f;
const char* image = "img.jpg";
// TextureFormats format = TextureFormats::JPG;

extern "C" {
    float GetX() {
        return x;
    }
     float GetY() {
        return y;
    }

    const char* GetTex() {
        return image;
    }

    float GetR(){
        return r;
    }

    float GetG(){
        return g;
    }

    float GetB(){
        return b;
    }

    //  TextureFormats GetFormat(){
    //     return format;
    //  }
}
