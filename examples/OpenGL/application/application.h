#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Event/event_handler.h>
#include <renderer.h>

#include <iostream>

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <Event/event_handler.h>
// #include <renderer.h>

// #include <iostream>

#include "../dep/glm/glm/mat4x4.hpp"
#include "../dep/glm/glm/vec2.hpp"
#include "../dep/glm/glm/gtc/matrix_transform.hpp"

// const glm::ivec2 SCREEN_DIMENSIONS(1000, 675);
// const glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0f));
// const glm::mat4 projectionMatrix = glm::ortho<float>(0.0f, SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, 0.0f, -0.1f, 100.0f);
// glm::mat4 trans = glm::mat4(1.0f);

// void processInput(GLFWwindow *window)
// {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);
// }

extern "C" {
    __declspec(dllexport) void OnEvent(Event* event, GLFWwindow* window, Renderer* renderer, glm::mat4* trans);
}
#endif

// #ifndef APPLICATION_H
// #define APPLICATION_H

// extern "C" {
//     __declspec(dllexport) void OnEvent(Event *event, GLFWwindow *window, Renderer *renderer,glm::mat4 trans);
//   //  __declspec(dllexport) void giveOutput(GLFWwindow *window, int texture, int shaderProgram, int VAO);
// }

// extern "C" {
//     __declspec(dllexport) void click(GLFWwindow *window, int button, int action, int mods);
//     __declspec(dllexport) void giveOutput(GLFWwindow *window, int texture, int shaderProgram, int VAO, float xx, float yy);
// }
// #endif