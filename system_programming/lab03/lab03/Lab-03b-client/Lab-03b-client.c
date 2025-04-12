#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {

    const char* str = argv[1];
    const char* name_pipe = (argc == 3) || (argc == 4) ? argv[2] : "defaultPipe";
    const char* name_semaphore = (argc == 4) ? argv[3] : "defaultSemaphore";

    char full_pipe_name[256];
    snprintf(full_pipe_name, sizeof(full_pipe_name), "\\\\.\\pipe\\%s", name_pipe);

    HANDLE hSemaphore = OpenSemaphore(SYNCHRONIZE, FALSE, name_semaphore);
    if (hSemaphore == NULL) {
        hSemaphore = CreateSemaphore(NULL, 1, 1, name_semaphore);
        if (hSemaphore == NULL) {
            printf("Error: Could not create semaphore '%s'. Error code: %lu\n", name_semaphore, GetLastError());
            return EXIT_FAILURE;
        }
        printf("Semaphore '%s' created by client.\n", name_semaphore);
    }
    else {
        printf("Semaphore '%s' opened by client.\n", name_semaphore);
    }

    DWORD waitResult = WaitForSingleObject(hSemaphore, INFINITE);
    if (waitResult != WAIT_OBJECT_0) {
        printf("Error: Failed to wait for semaphore. Error code: %lu\n", GetLastError());
        return EXIT_FAILURE;
    }

    HANDLE hPipe = CreateFile(
        full_pipe_name,
        GENERIC_WRITE | GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Error: Could not open pipe '%s'. Error code: %lu\n", name_pipe, GetLastError());
        return EXIT_FAILURE;
    }

    int randomSleepTime = rand() % 2000 + 1000;
    Sleep(randomSleepTime);
    DWORD bytesToSend = strlen(str) + 1;
    DWORD bytesWritten = 0;

    if (!WriteFile(hPipe, &bytesToSend, sizeof(bytesToSend), &bytesWritten, NULL)) {
        printf("Error: Failed to write length to pipe. Error code: %lu\n", GetLastError());
        ReleaseSemaphore(hSemaphore, 1, NULL);
        return EXIT_FAILURE;
    }

    if (!WriteFile(hPipe, str, strlen(str) + 1, &bytesWritten, NULL)) {
        printf("Pipe closed because of buffer overflow\n");
        ReleaseSemaphore(hSemaphore, 1, NULL);
        return EXIT_FAILURE;
    }
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    if (ReadFile(hPipe, buffer, BUFFER_SIZE - 1, &bytesRead, NULL)) {
        buffer[bytesRead] = '\0';
        printf("Received response: %s\n", buffer);
    }
    else {
        printf("Error: Failed to read from pipe. Error code: %lu\n", GetLastError());
    }

    ReleaseSemaphore(hSemaphore, 1, NULL);
    CloseHandle(hPipe);
    CloseHandle(hSemaphore);

    return EXIT_SUCCESS;
}