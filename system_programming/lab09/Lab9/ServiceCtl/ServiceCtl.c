#include <windows.h>
#include <stdio.h>
#include <tchar.h>

// Функции для управления сервисом
void CreateServiceCtl(const char* serviceName, const char* executablePath);
void StartServiceCtl(const char* serviceName, const char* monitoredDir, const char* logDir);
void StopServiceCtl(const char* serviceName);
void DeleteServiceCtl(const char* serviceName);
void PauseServiceCtl(const char* serviceName);
void ContinueServiceCtl(const char* serviceName);
void InfoServiceCtl(const char* serviceName);
void PrintError(const char* context);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: ServiceCtl <ServiceName> <Operation> [Additional Parameters]\n");
        return 1;
    }

    const char* serviceName = argv[1];
    const char* operation = argv[2];

    if (_stricmp(operation, "Create") == 0) {
        if (argc < 4) {
            printf("Error: Create operation requires the path to the executable.\n");
            return 1;
        }
        CreateServiceCtl(serviceName, argv[3]);
    } else if (_stricmp(operation, "Start") == 0) {
        if (argc < 5) {
            printf("Error: Start operation requires monitored directory and log directory.\n");
            return 1;
        }
        StartServiceCtl(serviceName, argv[3], argv[4]);
    } else if (_stricmp(operation, "Stop") == 0) {
        StopServiceCtl(serviceName);
    } else if (_stricmp(operation, "Delete") == 0) {
        DeleteServiceCtl(serviceName);
    } else if (_stricmp(operation, "Pause") == 0) {
        PauseServiceCtl(serviceName);
    } else if (_stricmp(operation, "Continue") == 0) {
        ContinueServiceCtl(serviceName);
    } else if (_stricmp(operation, "Info") == 0) {
        InfoServiceCtl(serviceName);
    } else {
        printf("Error: Unknown operation '%s'.\n", operation);
        return 1;
    }

    return 0;
}

void PrintError(const char* context) {
    DWORD errorCode = GetLastError();
    char* errorMessage = NULL;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        0,
        (LPSTR)&errorMessage,
        0,
        NULL
    );

    if (errorMessage) {
        printf("%s failed. Error %d: %s\n", context, errorCode, errorMessage);
        LocalFree(errorMessage);
    } else {
        printf("%s failed. Error %d.\n", context, errorCode);
    }
}

void CreateServiceCtl(const char* serviceName, const char* executablePath) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCManager) {
        PrintError("OpenSCManager");
        return;
    }

    SC_HANDLE hService = CreateService(
        hSCManager,
        serviceName,
        serviceName,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        executablePath,
        NULL, NULL, NULL, NULL, NULL
    );

    if (!hService) {
        PrintError("CreateService");
    } else {
        printf("Service '%s' created successfully.\n", serviceName);
        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}

void StartServiceCtl(const char* serviceName, const char* monitoredDir, const char* logDir) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        PrintError("OpenSCManager");
        return;
    }

    SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_START);
    if (!hService) {
        PrintError("OpenService");
    } else {
        const char* args[] = { monitoredDir, logDir };
        DWORD argc = 2;

        if (StartService(hService, argc, args)) {
            printf("Service '%s' started successfully.\n", serviceName);
        } else {
            PrintError("StartService");
        }

        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}

void StopServiceCtl(const char* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        PrintError("OpenSCManager");
        return;
    }

    SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_STOP);
    if (!hService) {
        PrintError("OpenService");
    } else {
        SERVICE_STATUS status;
        if (ControlService(hService, SERVICE_CONTROL_STOP, &status)) {
            printf("Service '%s' stopped successfully.\n", serviceName);
        } else {
            PrintError("ControlService");
        }

        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}

void DeleteServiceCtl(const char* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        PrintError("OpenSCManager");
        return;
    }

    SC_HANDLE hService = OpenService(hSCManager, serviceName, DELETE);
    if (!hService) {
        PrintError("OpenService");
    } else {
        if (DeleteService(hService)) {
            printf("Service '%s' deleted successfully.\n", serviceName);
        } else {
            PrintError("DeleteService");
        }

        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}

void PauseServiceCtl(const char* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        PrintError("OpenSCManager");
        return;
    }

    SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_PAUSE_CONTINUE);
    if (!hService) {
        PrintError("OpenService");
    } else {
        SERVICE_STATUS status;
        if (ControlService(hService, SERVICE_CONTROL_PAUSE, &status)) {
            printf("Service '%s' paused successfully.\n", serviceName);
        } else {
            PrintError("ControlService");
        }

        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}

void ContinueServiceCtl(const char* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        PrintError("OpenSCManager");
        return;
    }

    SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_PAUSE_CONTINUE);
    if (!hService) {
        PrintError("OpenService");
    } else {
        SERVICE_STATUS status;
        if (ControlService(hService, SERVICE_CONTROL_CONTINUE, &status)) {
            printf("Service '%s' continued successfully.\n", serviceName);
        } else {
            PrintError("ControlService");
        }

        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}

void InfoServiceCtl(const char* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        PrintError("OpenSCManager");
        return;
    }

    SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_QUERY_STATUS);
    if (!hService) {
        PrintError("OpenService");
    } else {
        SERVICE_STATUS_PROCESS ssp;
        DWORD bytesNeeded;
        if (QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssp, sizeof(ssp), &bytesNeeded)) {
            printf("Service '%s' information:\n", serviceName);
            printf("  State: %d\n", ssp.dwCurrentState);
        } else {
            PrintError("QueryServiceStatusEx");
        }

        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}