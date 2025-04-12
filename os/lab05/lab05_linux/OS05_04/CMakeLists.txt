cmake_minimum_required(VERSION 3.10)  # Укажите минимальную версию CMake
project(OS05_04)                      # Название вашего проекта

set(CMAKE_CXX_STANDARD 11)            # Установка стандарта C++
set(CMAKE_CXX_STANDARD_REQUIRED True)  # Обязательное использование указанного стандарта

add_executable(OS05_04 OS05_04.cpp)      # Определение исполняемого файла и его исходников