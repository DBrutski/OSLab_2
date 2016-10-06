project(neirons-network)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "-Wall  -Wpedantic -std=c11")
enable_testing()

add_subdirectory(lib ./lib)
add_subdirectory(unit_tests)
add_subdirectory(complex_tests)
add_subdirectory(queue_test)
add_subdirectory(collect_plots)
add_subdirectory(final_tests)

