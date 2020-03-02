cmake_minimum_required(VERSION 3.15)
project(MPI_test_1 C)

set(CMAKE_C_STANDARD 11)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

target_link_libraries(MPI_test_1 ${MPI_C_LIBRARIES})