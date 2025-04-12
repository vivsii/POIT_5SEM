#include <windows.h>
#include <stdio.h>
#include <wchar.h>

void createProcess(const char* appName) {
    PROCESS_INFORMATION processInfo;
    STARTUPINFO startupInfo;

    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));

    if (!CreateProcess(appName, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo)) {
        fprintf(stderr, "Error creating process for %s: %lu\n", appName, GetLastError());
    }
    else {
        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
    }
}

int main() {
    char currentDir[MAX_PATH];

    printf("lab02_hw-1.exe:\n");
    createProcess("lab02_hw-1.exe");
    Sleep(1000);

    // Выбранный каталог
    char chosenDirectory[MAX_PATH] = "C:\\SP02";

    if (GetFileAttributes(chosenDirectory) == INVALID_FILE_ATTRIBUTES) {
        fprintf(stderr, "Directory does not exist: %s\n", chosenDirectory);
        return 1;
    }

    if (SetCurrentDirectory(chosenDirectory) == 0) {
        fprintf(stderr, "Error setting chosen directory: %lu\n", GetLastError());
        return 1;
    }
    printf("lab02_hw-2.exe:\n");
    createProcess("lab02_hw-2.exe");
    Sleep(1000);

    // Системный каталог
    char systemDirectory[MAX_PATH];
    GetSystemDirectory(systemDirectory, sizeof(systemDirectory));
    if (SetCurrentDirectory(systemDirectory) == 0) {
        fprintf(stderr, "Error setting system directory: %lu\n", GetLastError());
        return 1;
    }
    printf("lab02_hw-3.exe:\n");
    createProcess("lab02_hw-3.exe");
    Sleep(1000);


    // Основной каталог Windows
    char windowsDirectory[MAX_PATH];
    GetWindowsDirectory(windowsDirectory, sizeof(windowsDirectory));
    if (SetCurrentDirectory(windowsDirectory) == 0) {
        fprintf(stderr, "Error setting main windows directory: %lu\n", GetLastError());
        return 1;
    }
    printf("lab02_hw-4.exe:\n");
    createProcess("lab02_hw-4.exe");
    Sleep(1000);

    // PATH
    char pathEnv[4096];
    DWORD result = GetEnvironmentVariable("PATH", pathEnv, sizeof(pathEnv));
    if (result == 0) {
        fprintf(stderr, "Error with PATH: %lu\n", GetLastError());
        return 1;
    }

    char* lastToken = NULL;
    char* token = strtok(pathEnv, ";");
    while (token != NULL) {
        lastToken = token;
        token = strtok(NULL, ";");
    }

    if (lastToken != NULL) {

        char fullPath[MAX_PATH];
        snprintf(fullPath, sizeof(fullPath), "%s", lastToken);
        if (SetCurrentDirectory(fullPath) == 0) {
            fprintf(stderr, "Error setting main windows directory: %lu\n", GetLastError());
            return 1;
        }
        printf("lab02_hw-5.exe:\n");
        createProcess("lab02_hw-5.exe");
        getchar();
    }
    else {
        printf("Unable to find last PATH directory.\n");
    }


    return 0;
}