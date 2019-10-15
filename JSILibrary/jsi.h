#pragma once

#ifdef JSILIBRARY_EXPORTS
#define JSILIBRARY_API __declspec(dllexport)
#else
#define JSILIBRARY_API __declspec(dllimport)
#endif // JSILIBRARY_EXPORTS


extern "C" JSILIBRARY_API void InterpretText(const char* variables, const char* values, const char* functions, const char* textToInterpret, char* result, int resLength);
extern "C" JSILIBRARY_API void InterpretTextEx(const char* variables[], const char* values[], int nbVar, const char* functions[], int nbFunc, const char* textToInterpret, char* result, int resLength);
