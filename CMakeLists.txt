project(neirons-network)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "-Wall  -Wpedantic -std=c11")
enable_testing()

#add_subdirectory(test ./test)
add_subdirectory(lib ./lib)
add_subdirectory(unit_tests)
add_subdirectory(stress_tests)
#add_subdirectory(tests_on_c)


