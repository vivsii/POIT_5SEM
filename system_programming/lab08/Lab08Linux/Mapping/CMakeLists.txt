cmake_minimum_required(VERSION 3.25)
project(Mapping)

add_library(Mapping STATIC Mapping.c)

target_include_directories(Mapping PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
