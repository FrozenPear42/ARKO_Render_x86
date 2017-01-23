cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME ARKO_Render_x86)
set(CMAKE_PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_C_FLAGS -m64)
set(CMAKE_CXX_FLAGS -m64)
enable_language(ASM_NASM)

set(SOURCE_FILES main.cpp render.asm)
set(EXECUTABLE_NAME ${PROJECT_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

find_library(SFML 2.3)
include_directories(${SFML_ROOT}/include)
target_link_libraries(${EXECUTABLE_NAME}
        ${SFML_ROOT}/libs/libsfml-system.so
        ${SFML_ROOT}/libs/libsfml-window.so
        ${SFML_ROOT}/libs/libsfml-graphics.so)
