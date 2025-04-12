#include <iostream>
#include <thread>
#include <windows.h>

int main() {

    DWORD processID = GetCurrentProcessId();

    while (true) {

        DWORD threadID = GetCurrentThreadId();
        std::cout << "Process ID: " << processID << ", Thread ID: " << threadID << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}