project(collect_plots)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} collect_plots.c)
target_link_libraries(${PROJECT_NAME} mmemory-lib)


