cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Project1 C)

# Установка стандарта языка C
set(CMAKE_C_STANDARD 99)

# Добавляем исполняемый файл
add_executable(StrTypeChecker Project1/lab.c)

# Используем компилятор Clang
set(CMAKE_C_COMPILER clang)