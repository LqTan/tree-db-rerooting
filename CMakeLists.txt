cmake_minimum_required(VERSION 3.20)

project(tree_dp_rerooting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main
    src/main.cpp
)
add_executable(test
    src/test.cpp
)
add_executable(tree_matching
    src/tree_matching.cpp
)
add_executable(tree_rerooting
    src/tree_rerooting.cpp
)

target_include_directories(main PRIVATE src)
target_include_directories(test PRIVATE src)
target_include_directories(tree_matching PRIVATE src)
target_include_directories(tree_rerooting PRIVATE src)