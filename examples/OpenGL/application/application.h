#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Event/event_handler.h>
#include <renderer.h>
#include <string>

#include <iostream>

extern "C" {

    extern float x;
    extern float y;
    extern std::string image;

    __declspec(dllexport) float GetX();
    __declspec(dllexport) float GetY();
    __declspec(dllexport) std::string GetTex();

}
#endif