set(CMAKE_BUILD_TYPE Debug)

add_library( mmemory-lib mmemory.h mmemory.cpp memory_dispatcher.cpp memory_dispatcher.h segment.cpp segment.h page.cpp page.h memory_pager.cpp memory_pager.h)

target_include_directories(mmemory-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


