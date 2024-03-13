#pragma once

#ifdef _WIN32
#include <windows.h>
#define OS_HANDLE HMODULE
#define FPTR FARPROC
#define lib_find(handle, symbol) GetProcAddress(handle, symbol)

#define dril_close(handle) FreeLibrary(handle)
#define dril_load(handle) LoadLibraryA(handle)
#define dril_reload() Load(1)
#define popen _popen
#define pcose _pclose

#elif __linux__
#include <dlfcn.h>
#define OS_HANDLE void *
#define FPTR void *
#define lib_find(handle, symbol) dlsym(handle, symbol)
#define dril_close(handle) dlclose(handle)
#define dril_load(handle) dlopen(handle, RTLD_NOW)
#define dril_reload() Load(0)

#elif __APPLE__
#include <dlfcn.h>
#define OS_HANDLE void *
#define FPTR void *
#define lib_find(handle, symbol) dlsym(handle, symbol)
#define dril_close(handle) dlclose(handle)
#define dril_load(handle) dlopen(handle, RTLD_NOW)
#define dril_reload() Load(0)

#endif