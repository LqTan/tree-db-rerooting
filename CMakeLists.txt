cmake_minimum_required(VERSION 3.20)

project(tree_dp_rerooting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tree_dp
    src/main.cpp
)

target_include_directories(tree_dp PRIVATE src)