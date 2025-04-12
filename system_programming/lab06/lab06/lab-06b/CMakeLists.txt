cmake_minimum_required(VERSION 3.10)
project(lab-06b C)  

# Добавление исходных файлов
add_executable(lab-06b lab-06b.c)  

# Для MSVC компилятора добавляем флаг для консольного приложения
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()