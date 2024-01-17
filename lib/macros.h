#pragma once
#ifdef _WIN32
#include <windows.h>
#define OS_HANDLE HMODULE
#define fptr FARPROC
#define lib_find(handle,symbol) GetProcAddress(handle,symbol)
#define drilcp while(!copyFile(GetPath(),CopyPath())){ Sleep(10);}
#define dril_close(handle) FreeLibrary(handle)
#define dril_load(handle) LoadLibraryA(handle)
#define dril_reload() Load(0)
#elif __linux__
#include <dlfcn.h>
#define OS_HANDLE void*
#define fptr void*
#define lib_find((handle,symbol) dlsym((handle,symbol)
#define drilcp
#define dril_close(handle) dlclose(handle)
#define dril_load(handle) dlopen(handle)
#define dril_reload() Load(1)
#elif __APPLE__
#include <dlfcn.h>
#define OS_HANDLE void*
#define fptr void*
#define lib_find((handle,symbol) dlsym((handle,symbol)
#define drilcp
#define dril_close(handle) dlclose(handle)
#define dril_load(handle) dlopen(handle)
#define dril_reload() Load(1)
#endif


