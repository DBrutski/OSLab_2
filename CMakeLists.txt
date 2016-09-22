project(neirons-network)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "-Wwrite-strings")
add_definitions(-Wwrite-strings)

set(CMAKE_CXX_FLAGS "-Wwrite-strings")

add_subdirectory(test ./test)
add_subdirectory(lib ./lib)
add_subdirectory(unit_tests)


