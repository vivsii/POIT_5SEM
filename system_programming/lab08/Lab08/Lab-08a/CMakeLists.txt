cmake_minimum_required(VERSION 3.10)
project(Lab_08a)

add_executable(Lab_08a Lab-08a.c)

target_link_libraries(Lab_08a PRIVATE Mapping)