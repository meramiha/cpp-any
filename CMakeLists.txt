cmake_minimum_required(VERSION 3.27)
project(cpp_any)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(cpp_any test.cpp
        any.cpp
        any.h)
