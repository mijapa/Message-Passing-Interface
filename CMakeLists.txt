cmake_minimum_required(VERSION 3.15)
project(MPI_test_1 C)

set(CMAKE_C_STANDARD 11)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

add_executable(various "mpi_testing_various_p2p_communication_methods.c")
add_executable(delay "mpi_testing_communication_delay.c")
add_executable(hello "mpi_hello_proc.c")
add_executable(throughput "mpi_testing_standard_send_throughput.c")
add_executable(buffered_throughput "mpi_testing_buffered_send_throughput.c")

target_link_libraries(various ${MPI_C_LIBRARIES})
