# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(OS04_11)

# Добавляем поддержку потоков
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Добавляем исполняемый файл
add_executable(OS04_11 OS04_11.c)

# Линковка потоков
target_link_libraries(OS04_11 ${CMAKE_THREAD_LIBS_INIT})