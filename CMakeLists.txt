cmake_minimum_required(VERSION 3.5)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there")
endif()

project(ppmd)

add_subdirectory(libppmd)
add_subdirectory(libppmdlua)
add_subdirectory(ppmd)
add_subdirectory(plugins)
