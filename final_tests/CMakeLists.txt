project(final_tests)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} final_unit_tests.c)
target_link_libraries(${PROJECT_NAME} mmemory-lib)


