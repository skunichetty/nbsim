cmake_minimum_required(VERSION 3.8)
project(nbsim VERSION 0.1.0 LANGUAGES CXX)

add_executable(nbsim src/main.cpp
    src/body.cpp
    src/engine.cpp
    src/io_handler.cpp
    src/main.cpp
    src/object.cpp
    src/octree.cpp
    src/octree_node.cpp
    src/vec3.cpp
)

target_include_directories(nbsim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/nbsim)