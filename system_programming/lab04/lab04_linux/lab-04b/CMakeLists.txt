cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Lab04Threads)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем исполняемый файл
add_executable(lab-04b lab-04b.cpp L4Thread.cpp L4Thread.h)
