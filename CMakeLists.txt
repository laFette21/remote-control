cmake_minimum_required(VERSION 3.12)
project(Remote Control)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(
    -Wall
    -g
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

enable_testing()

add_subdirectory(src)
