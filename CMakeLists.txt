cmake_minimum_required(VERSION 3.1)
project(Tebex_Arma3)

ADD_LIBRARY( libcurl SHARED IMPORTED GLOBAL)
set_target_properties( libcurl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libcurl.lib )

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(PkgConfig)
include_directories (include)

add_library(Tebex_Arma3 SHARED library.cpp include/extension.h extension.cpp Config.cpp Config.h Api.cpp Api.h)

TARGET_LINK_LIBRARIES(Tebex_Arma3 libcurl)

