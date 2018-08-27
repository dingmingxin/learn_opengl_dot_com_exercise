cmake_minimum_required (VERSION 3.3)

project (glad)

add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
