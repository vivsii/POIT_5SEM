cmake_minimum_required(VERSION 3.10)
project(Mappingd)

add_library(Mappingd SHARED Mappingd.c)

target_include_directories(Mappingd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
