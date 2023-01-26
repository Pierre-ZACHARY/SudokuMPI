cmake_minimum_required(VERSION 3.9.1)
project(parallel_task)

set(CMAKE_CXX_STANDARD 14)

# -fopenmp flag (enables OpenMP)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -lm -O3 -funroll-loops -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(parallel_task fonctions.cpp fonctions.h main.cpp)

# MPI
find_package(MPI REQUIRED)
target_link_libraries(parallel_task PUBLIC MPI::MPI_CXX)
