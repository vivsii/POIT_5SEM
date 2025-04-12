#include "L4Thread.h"
#include <vector>
#include <Windows.h>
#include <iostream>

extern std::vector<int> global_primes;
extern CRITICAL_SECTION cs;
DWORD tlsIndex;

bool InitializeTLS() {
    tlsIndex = TlsAlloc();
    return tlsIndex != TLS_OUT_OF_INDEXES;
}

void CleanupTLS() {
    TlsFree(tlsIndex);
}

DWORD WINAPI L4Thread(LPVOID param) {
    int start = ((int*)param)[0];
    int end = ((int*)param)[1];


    std::vector<int>* local_primes = new std::vector<int>();
    if (!TlsSetValue(tlsIndex, local_primes)) {
        std::cerr << "Error setting TLS value: " << GetLastError() << std::endl;
        delete[](int*)param;
        delete local_primes;
        return 1;
    }

    for (int num = start; num <= end; ++num) {
        bool is_prime = true;
        if (num < 2) continue;
        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) local_primes->push_back(num);
    }

    EnterCriticalSection(&cs);
    global_primes.insert(global_primes.end(), local_primes->begin(), local_primes->end());
    LeaveCriticalSection(&cs);

    delete local_primes;
    delete[](int*)param;
    return 0;
}
