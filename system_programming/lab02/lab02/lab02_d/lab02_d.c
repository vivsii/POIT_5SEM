#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    si.cb = sizeof(si);
    printf("Press any key to terminate the child process...\n");
    Sleep(2000);
    getchar();
    if (!CreateProcess(NULL, "lab02_inf.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }
    Sleep(3000);


    TerminateProcess(pi.hProcess, 0);

    printf("Child PID: %d\n", pi.dwProcessId);
    getchar();

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("Child process terminated. Press any key to continue...\n");
    getchar();

    printf("Child PID: %d\n", pi.dwProcessId);
    getchar();

    printf("Press any key to exit...\n");
    getchar();

    return 0;
}