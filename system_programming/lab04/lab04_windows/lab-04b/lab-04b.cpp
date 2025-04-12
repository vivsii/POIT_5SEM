#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm> 
#include <string>
#include <Windows.h>
#include "L4Thread.h"

std::vector<int> global_primes;
CRITICAL_SECTION cs;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: Lab-04b <num_threads> <lower_bound> <upper_bound>\n";
        return 1;
    }

    int num_threads = std::stoi(argv[1]);
    int lower_bound = std::stoi(argv[2]);
    int upper_bound = std::stoi(argv[3]);

    InitializeCriticalSection(&cs);

    int range = (upper_bound - lower_bound + 1) / num_threads;
    std::vector<HANDLE> threads;

    for (int i = 0; i < num_threads; ++i) {
        int start = lower_bound + i * range;
        int end = (i == num_threads - 1) ? upper_bound : start + range - 1;
        int* args = new int[2] { start, end };
        HANDLE hThread = CreateThread(nullptr, 0, L4Thread, args, 0, nullptr);
        threads.push_back(hThread);
    }

    WaitForMultipleObjects(num_threads, threads.data(), TRUE, INFINITE);

    std::sort(global_primes.begin(), global_primes.end());
    for (int prime : global_primes) {
        std::cout << prime << " ";
    }

    for (HANDLE hThread : threads) {
        CloseHandle(hThread);
    }
    DeleteCriticalSection(&cs);

    return 0;
}
    