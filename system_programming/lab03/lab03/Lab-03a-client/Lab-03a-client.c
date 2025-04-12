#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

#define MAX_PRIMES 1024

bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <Lower Bound> <Upper Bound> <Mutex Name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);
    const char* mutex_name = argv[3];

    HANDLE hWrite = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hWrite == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error getting standard output handle: %lu\n", GetLastError());
        return EXIT_FAILURE;
    }

    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutex_name);
    if (hMutex == NULL) {
        fprintf(stderr, "Error opening mutex '%s': %lu\n", mutex_name, GetLastError());
        return EXIT_FAILURE;
    }

    int primes[MAX_PRIMES];
    int count = 0;

    for (int i = lower; i <= upper; i++) {
        if (is_prime(i) && count < MAX_PRIMES) {
            primes[count++] = i;
        }
    }

    WaitForSingleObject(hMutex, INFINITE);

    DWORD written;
    if (WriteFile(hWrite, &count, sizeof(count), &written, NULL) && written == sizeof(count)) {
        ReleaseMutex(hMutex);
        Sleep(1000);
        WaitForSingleObject(hMutex, INFINITE);
        WriteFile(hWrite, primes, sizeof(int) * count, &written, NULL);
    }
    else {
        fprintf(stderr, "Error writing primes to pipe\n");
    }

    ReleaseMutex(hMutex);
    CloseHandle(hWrite);
    CloseHandle(hMutex);

    return EXIT_SUCCESS;
}
