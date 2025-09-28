cmake_minimum_required(VERSION 3.25)

project(Simverse
    VERSION 0.1.0
    LANGUAGES CXX)

# language standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(simverse_server src/main.cpp src/world.cpp)

target_include_directories(simverse_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/proto)


if (MSVC)
    target_compile_options(simverse_server PRIVATE /W4 /permissive-)
else()
    target_compile_options(simverse_server PRIVATE -Wall -Wextra -Wpedantic)
endif()



