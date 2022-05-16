cmake_minimum_required(VERSION 3.8) # specify min version
project(nbsim VERSION 0.1.0 LANGUAGES CXX) # name of project + language used

# Enables tests
set(BUILD_TESTS True)

# main executable file
add_executable(nbsim src/main.cpp
    src/body.cpp
    src/engine.cpp
    src/io_handler.cpp
    src/main.cpp
    src/object.cpp
    src/octree.cpp
    src/octree_node.cpp
    src/bounding_box.cpp
)

# specifies include directory for main executable
target_include_directories(nbsim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/nbsim)
# compile options for main executable
target_compile_options(nbsim PUBLIC -pg -O3 -std=c++17)

add_executable(nbsim_tests test/test.cpp
    test/octree_node_tests.cpp
    test/vec3_tests.cpp
    test/octree_tests.cpp
    src/vec3.cpp
    src/octree_node.cpp
    src/bounding_box.cpp
    src/object.cpp
    src/octree.cpp
    src/body.cpp
)
target_include_directories(nbsim_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/test)
target_include_directories(nbsim_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/nbsim)