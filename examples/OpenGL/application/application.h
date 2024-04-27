#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Event/event_handler.h>
#include <renderer.h>

#include <iostream>

extern "C" {

    extern float x;
    extern float y;
    extern const char* image;
    // extern TextureFormats format;

    DRIL_EXPORT float GetX();
    DRIL_EXPORT float GetY();
    DRIL_EXPORT float GetR();
    DRIL_EXPORT float GetG();
    DRIL_EXPORT float GetB();
    DRIL_EXPORT const char* GetTex();
    // DRIL_EXPORT TextureFormats GetFormat();

}
#endif