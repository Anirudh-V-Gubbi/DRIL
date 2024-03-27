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
    // extern TextureFormats format;

    __declspec(dllexport) float GetX();
    __declspec(dllexport) float GetY();
    __declspec(dllexport) float GetR();
    __declspec(dllexport) float GetG();
    __declspec(dllexport) float GetB();
    __declspec(dllexport) std::string GetTex();
    // __declspec(dllexport) TextureFormats GetFormat();

}
#endif