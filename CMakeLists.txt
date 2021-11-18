cmake_minimum_required(VERSION 3.20)
project(Vector VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES Vector.c Vector.h)
add_library(Vector STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_link_libraries(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})