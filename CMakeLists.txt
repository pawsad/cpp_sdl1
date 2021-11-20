cmake_minimum_required(VERSION 3.10)

project(cpp_sdl1 CXX)

set(SOURCES sdl_draw_01.cpp)
set(HEADERS sdl_draw_01.h)


#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} "main.cpp")
target_link_libraries(${PROJECT_NAME}
    SDL2
)
