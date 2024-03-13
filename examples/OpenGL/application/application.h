#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Event/event_handler.h>
#include <renderer.h>

#include <iostream>

extern "C" {
    void OnEvent(Event* event, GLFWwindow* window, Renderer* renderer);
}
#endif