#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

void OS04_02_T1() {
    DWORD processID = GetCurrentProcessId();
    for (int i = 0; i < 50; ++i) {
        DWORD threadID = GetCurrentThreadId();
        std::cout << "Thread T1 - Process ID: " << processID << ", Thread ID: " << threadID << ", Iteration: " << i + 1 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void OS04_02_T2() {
    DWORD processID = GetCurrentProcessId();
    for (int i = 0; i < 125; ++i) {
        DWORD threadID = GetCurrentThreadId();
        std::cout << "Thread T2 - Process ID: " << processID << ", Thread ID: " << threadID << ", Iteration: " << i + 1 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    DWORD processID = GetCurrentProcessId();
    std::cout << "Process ID: " << processID << std::endl;

    // Запускаем потоки
    std::thread thread1(OS04_02_T1);
    std::thread thread2(OS04_02_T2);

    // Цикл 100 итераций с задержкой 1 секунда
    for (int i = 0; i < 100; ++i) {
        std::cout << "Main Thread - Process ID: " << processID << ", Iteration: " << i + 1 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Ждем завершения потоков
    thread1.join();
    thread2.join();

    return 0;
}