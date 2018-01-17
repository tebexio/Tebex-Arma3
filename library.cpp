#include "extension.h"

#ifndef TEBEX_CONSOLE

// Windows
#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        tebex::extension::initialize();
        break;

    case DLL_PROCESS_DETACH:
        tebex::extension::finalize();
        break;
    }
    return true;
}

extern "C" {
    __declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
};

void __stdcall RVExtension(char *output, int outputSize, const char *function) {
    outputSize -= 1;
    tebex::extension::call(output, outputSize, function);
};

// Linux with GCC
#else

extern "C" {
void RVExtension(char *output, int outputSize, const char *function);
}

void RVExtension(char *output, int outputSize, const char *function) {
    outputSize -= 1;
    tebex::extension::call(output, outputSize, function);
}

__attribute__((constructor))
static void extension_init() {
    tebex::extension::initialize();
}

__attribute__((destructor))
static void extension_finalize() {
    tebex::extension::finalize();
}

#endif

#else

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string line = "";
    const int outputSize = 10000;
    char *output = new char[outputSize];

    tebex::extension::initialize();
    std::cout
        << "Type 'exit' to close console." << std::endl
        << std::endl << std::endl;
    while (line != "exit") {
        std::getline(std::cin, line);
        tebex::extension::call(output, outputSize, line.c_str());
        std::cout << "TEBEX: " << output << std::endl;
    }
    tebex::extension::finalize();
    return 0;
}

#endif