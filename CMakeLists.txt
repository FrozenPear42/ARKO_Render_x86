cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME ARKO_Render_x86)
set(CMAKE_PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_CXX_COMPILER g++-4.9)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
set(CMAKE_C_FLAGS -m32)
set(CMAKE_CXX_FLAGS -m32)
enable_language(ASM_NASM)

set(SOURCE_FILES main.cpp render.asm)
set(EXECUTABLE_NAME ${PROJECT_NAME})
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
set(SFML_ROOT ./SFML)
#find_package(SFML 2 REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME}
                            ${CMAKE_SOURCE_DIR}/SFML/lib/libsfml-system.so
                            ${CMAKE_SOURCE_DIR}/SFML/lib/libsfml-window.so
                            ${CMAKE_SOURCE_DIR}/SFML/lib/libsfml-graphics.so )


