#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

#define BUFFER_SIZE 256

void to_uppercase(char* str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int main(int argc, char* argv[]) {

    const char* pipe_name = (argc == 2) ? argv[1] : "defaultPipe";
    char full_pipe_name[256];
    snprintf(full_pipe_name, sizeof(full_pipe_name), "\\\\.\\pipe\\%s", pipe_name);
    HANDLE hPipe = CreateNamedPipe(full_pipe_name, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Error: Could not create pipe '%s'. Error code: %lu\n", full_pipe_name, GetLastError());
        return EXIT_FAILURE;
    }

    printf("Server is running and waiting for clients...\n");

    while (true) {
        if (!ConnectNamedPipe(hPipe, NULL)) {
            printf("Error: Failed to connect to pipe. Error code: %lu\n", GetLastError());
            CloseHandle(hPipe);
            return EXIT_FAILURE;
        }
        char buffer[BUFFER_SIZE];
        DWORD bytesRead;

        DWORD bytesToRead;
        if (!ReadFile(hPipe, &bytesToRead, sizeof(bytesToRead), &bytesRead, NULL)) {
            printf("Error: Failed to read length from pipe. Error code: %lu\n", GetLastError());
        }
        if (bytesToRead > BUFFER_SIZE)
        {
            printf("Error: Buffer overflow\n");
            DisconnectNamedPipe(hPipe);
            continue;
        }

        if (ReadFile(hPipe, buffer, BUFFER_SIZE - 1, &bytesRead, NULL)) {
            buffer[bytesRead] = '\0';
            printf("Received string: %s\n", buffer);
            to_uppercase(buffer);
            printf("Transformed string: %s\n", buffer);

            DWORD bytesWritten;
            if (!WriteFile(hPipe, buffer, strlen(buffer) + 1, &bytesWritten, NULL))
                printf("Error: Failed to write to pipe. Error code: %lu\n", GetLastError());
        }
        else {
            fprintf(stderr, "Error: Failed to read from pipe. Error code: %lu\n", GetLastError());
        }
        DisconnectNamedPipe(hPipe);
    }
    CloseHandle(hPipe);
    return EXIT_SUCCESS;

}