project(test_paging)
cmake_minimum_required(VERSION 2.8)

add_executable(${PROJECT_NAME} test_paging.c)
target_link_libraries(${PROJECT_NAME} mmemory-lib)


