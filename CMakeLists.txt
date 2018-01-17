cmake_minimum_required(VERSION 3.9)
project(Tebex_Arma3)

set(CMAKE_CXX_STANDARD 11)
FIND_PACKAGE(PkgConfig)
include_directories (include)

add_library(Tebex_Arma3 SHARED library.cpp include/extension.h extension.cpp)