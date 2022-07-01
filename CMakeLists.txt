cmake_minimum_required(VERSION 3.12)
project(remote-control)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_compile_options(
    -g
    -Wall
)

if(COVERAGE)
    add_compile_options(
        --coverage
        -O0
    )
    add_link_options(--coverage)
else()
    add_compile_options(-O3)
endif()

find_package(Boost REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Pylon REQUIRED)

enable_testing()

add_subdirectory(src)
