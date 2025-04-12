#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_PROCESSES 3

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Enter number of iterations.\n");
        return 1;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi[NUM_PROCESSES];
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(pi, sizeof(pi));
    si.cb = sizeof(si);

    const char* executable = "lab02_x_modified.exe";
    char commandLine[MAX_PATH];

    // только имя программы
    snprintf(commandLine, sizeof(commandLine), "\"%s\"", executable);
    printf("Creating 1st process...\n");
    if (!CreateProcess(executable, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi[0])) {
        printf("Failed to create 1st process (%d).\n", GetLastError());
    }
    Sleep(3000);

    // только параметр
    snprintf(commandLine, sizeof(commandLine), "%s", argv[1]);
    printf("Creating 2nd process...\n");
    snprintf(commandLine, sizeof(commandLine), "5"); // 5 - количество итераций
    if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi[1])) {
        printf("Failed to create 2nd process (%d).\n", GetLastError());
    }
    Sleep(3000);


    if (SetEnvironmentVariable("ITER_NUM", "10") == 0) {
        printf("Failed to set environment variable (%d).\n", GetLastError());
    }

    snprintf(commandLine, sizeof(commandLine), "\"%s\" %s", executable, NULL);
    printf("Creating 3rd process...\n");
    if (!CreateProcess(executable, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi[2])) {
        printf("Failed to create 3rd process (%d).\n", GetLastError());
    }
    Sleep(1500);

    WaitForSingleObject(pi[0].hProcess, INFINITE);
    WaitForSingleObject(pi[1].hProcess, INFINITE);
    WaitForSingleObject(pi[2].hProcess, INFINITE);

    for (int i = 0; i < NUM_PROCESSES; i++) {
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }

    return 0;
}