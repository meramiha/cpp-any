cmake_minimum_required(VERSION 3.27)
project(cpp_any)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

find_package(Boost 1.36.0)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(cpp_any test.cpp
            any.cpp
            any.h)
endif ()


