// #ifndef APPLICATION_H
// #define APPLICATION_H

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <Event/event_handler.h>
// #include <renderer.h>

// #include <iostream>

// extern "C" {
//     __declspec(dllexport) void OnEvent(Event* event, GLFWwindow* window, Renderer* renderer);
// }
// #endif

#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Event/event_handler.h>
#include <renderer.h>

#include <iostream>

extern "C" {
    __declspec(dllexport) void OnEvent(Event* event, GLFWwindow* window, Renderer* renderer);
   // __declspec(dllexport) void giveOutput(GLFWwindow *window, int texture, int shaderProgram, int VAO);
}
#endif