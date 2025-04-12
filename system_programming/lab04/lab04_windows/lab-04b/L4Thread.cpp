#include "L4Thread.h"
#include <vector>
#include <Windows.h>

extern std::vector<int> global_primes;
extern CRITICAL_SECTION cs;

__declspec(thread) std::vector<int> local_primes;

DWORD WINAPI L4Thread(LPVOID param) {
    int start = ((int*)param)[0];
    int end = ((int*)param)[1];

    for (int num = start; num <= end; ++num) {
        bool is_prime = true;
        if (num < 2) continue;
        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) local_primes.push_back(num);
    }

    EnterCriticalSection(&cs);
    global_primes.insert(global_primes.end(), local_primes.begin(), local_primes.end());
    LeaveCriticalSection(&cs);

    delete[](int*)param;
    return 0;
}