project(neirons-network)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic -std=c11")
enable_testing()

#add_subdirectory(test ./test)
add_subdirectory(lib ./lib)
add_subdirectory(unit_tests)
add_subdirectory(complex_tests)
add_subdirectory(queue_test)
#add_subdirectory(tests_on_c)
add_subdirectory(test_paging)


