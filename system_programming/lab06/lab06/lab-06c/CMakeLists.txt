cmake_minimum_required(VERSION 3.10)
project(lab-06c C)  

# Добавление исходных файлов
add_executable(lab-06c lab-06c.c)  

# Для MSVC компилятора добавляем флаг для консольного приложения
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()