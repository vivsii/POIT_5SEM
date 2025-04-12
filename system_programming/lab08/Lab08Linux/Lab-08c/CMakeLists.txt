cmake_minimum_required(VERSION 3.10)

project(Lab-08c)

add_executable(lab-08c lab-08c.c)

target_link_libraries(lab-08c PUBLIC Mapping)

target_include_directories(lab-08c PUBLIC ${CMAKE_SOURCE_DIR}/Mapping)
