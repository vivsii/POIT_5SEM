#include <iostream>
#include <windows.h>
#include <bitset>
#include <winbase.h>

int main() {
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();

    int processPriority = GetPriorityClass(hProcess);
    int threadPriority = GetThreadPriority(hThread);
    DWORD_PTR affinityMask, systemAffinityMask;

    GetProcessAffinityMask(hProcess, &affinityMask, &systemAffinityMask);
    int numProcessors = 0;

    for (int i = 0; i < sizeof(DWORD_PTR) * 8; ++i) {
        if (affinityMask & (1 << i)) {
            numProcessors++;
        }
    }

    DWORD_PTR currentProcessor = GetCurrentProcessorNumber();

    std::cout << "PID: " << processId << std::endl;
    std::cout << "PIT: " << threadId << std::endl;
    std::cout << "PRIORITY OF PROC: " << processPriority << std::endl;
    std::cout << "PRIORITY OF THREAD: " << threadPriority << std::endl;

    std::cout << "AFFINITY MASK: ";
    std::cout << std::bitset<sizeof(DWORD_PTR) * 8>(affinityMask) << std::endl;

    std::cout << "N accesable proc: " << numProcessors << std::endl;
    std::cout << "Proc pf current thread: " << currentProcessor << std::endl;

    CloseHandle(hProcess);
    CloseHandle(hThread);
    return 0;
}