cmake_minimum_required (VERSION 3.21)

add_library(${PROJECT_NAME} window.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC window.h)