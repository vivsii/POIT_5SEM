cmake_minimum_required(VERSION 3.10)

# Укажите имя проекта
project(lab-07a)

# Укажите стандарт C
set(CMAKE_C_STANDARD 11)

# Укажите исполняемый файл
add_executable(lab-07a lab-07a.c)

# Укажите дополнительные параметры компилятора (при необходимости)
if(MSVC)
    target_compile_options(lab-07a PRIVATE /W4)  # Для Windows используем MSVC
else()
    target_compile_options(lab-07a PRIVATE -Wall -Wextra)  # Для других компиляторов
endif()