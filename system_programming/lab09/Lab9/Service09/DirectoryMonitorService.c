#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <direct.h>
#include <time.h>
#include <shlwapi.h> // Для PathFileExists
#pragma comment(lib, "Shlwapi.lib")

char service_name[] = "DirectoryMonitorService";

SERVICE_STATUS service_status;
SERVICE_STATUS_HANDLE hServiceStatus;

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI ServiceCtrlHandler(DWORD dwControl);
void WriteLog(const char *filePath, const char *format, ...);

HANDLE hDir = NULL;
char srv_log_file[MAX_PATH];
char dir_log_file[MAX_PATH];
char monitored_dir[MAX_PATH];
HANDLE hStopEvent = NULL;
HANDLE hPauseEvent = NULL;

#define DEFAULT_MONITORED_DIR "C:\\temp\\monitored"
#define DEFAULT_LOG_DIR "C:\\temp\\logs"

// Функция для создания директорий рекурсивно
int CreateDirectoriesRecursively(const char* dir) {
    if (PathFileExists(dir)) {
        return 0; // Директория уже существует
    }

    char temp[MAX_PATH];
    strcpy(temp, dir);

    for (char* p = temp + 1; *p; p++) {
        if (*p == '\\' || *p == '/') {
            *p = '\0';
            if (!PathFileExists(temp)) {
                if (!CreateDirectory(temp, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
                    return -1; // Ошибка создания
                }
            }
            *p = '\\';
        }
    }

    if (!CreateDirectory(temp, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
        return -1; // Ошибка создания
    }

    return 0;
}

int main() {
    SERVICE_TABLE_ENTRY service_table[] = {
        {service_name, ServiceMain},
        {NULL, NULL}
    };

    if (!StartServiceCtrlDispatcher(service_table)) {
        printf("Failed to start service control dispatcher.\n");
        return 1;
    }

    return 0;
}

void WriteLog(const char *filePath, const char *format, ...) {
    FILE *file = fopen(filePath, "a");
    if (!file) {
        return;
    }

    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    fprintf(file, "[%02d:%02d:%02d] ", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fclose(file);
}

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv) {
    hServiceStatus = RegisterServiceCtrlHandler(service_name, ServiceCtrlHandler);
    if (!hServiceStatus) {
        return;
    }

    service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    service_status.dwCurrentState = SERVICE_START_PENDING;
    service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
    service_status.dwWin32ExitCode = NO_ERROR;
    service_status.dwServiceSpecificExitCode = 0;
    service_status.dwCheckPoint = 0;
    service_status.dwWaitHint = 5000;

    SetServiceStatus(hServiceStatus, &service_status);

    strcpy(monitored_dir, DEFAULT_MONITORED_DIR);
    char log_dir[MAX_PATH];
    strcpy(log_dir, DEFAULT_LOG_DIR);

    if (CreateDirectoriesRecursively(log_dir) != 0) {
        WriteLog("C:\\temp\\service_error.log", "Failed to create log directory %s.\n", log_dir);
        service_status.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &service_status);
        return;
    } else {
        sprintf(srv_log_file, "%s\\%lld-srv.log", log_dir, time(NULL));
        WriteLog(srv_log_file, "Directory %s created or already exists for logging.\n", log_dir);
    }

    sprintf(dir_log_file, "%s\\%lld-dir.log", log_dir, time(NULL));

    if (CreateDirectoriesRecursively(monitored_dir) != 0) {
        WriteLog(srv_log_file, "Failed to create monitored directory: %s.\n", monitored_dir);
        service_status.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &service_status);
        return;
    } else {
        WriteLog(srv_log_file, "Monitored directory %s created or already exists.\n", monitored_dir);
    }

    WriteLog(srv_log_file, "Service started with default parameters: %s, %s\n", monitored_dir, log_dir);
    service_status.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hServiceStatus, &service_status);

    hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    hPauseEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

    hDir = CreateFile(
        monitored_dir,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        WriteLog(srv_log_file, "Failed to open directory for monitoring: %s\n", monitored_dir);
        service_status.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &service_status);
        return;
    }

    char buffer[1024];
    DWORD bytesReturned;
    FILE_NOTIFY_INFORMATION *notify;

    while (WaitForSingleObject(hStopEvent, 0) != WAIT_OBJECT_0) {
        WaitForSingleObject(hPauseEvent, INFINITE);

        if (ReadDirectoryChangesW(
                hDir,
                buffer,
                sizeof(buffer),
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
                    FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE,
                &bytesReturned,
                NULL,
                NULL)) {
            notify = (FILE_NOTIFY_INFORMATION *)buffer;
            do {
                char fileName[MAX_PATH];
                int count = WideCharToMultiByte(
                    CP_ACP, 0, notify->FileName, notify->FileNameLength / sizeof(WCHAR),
                    fileName, sizeof(fileName) - 1, NULL, NULL);
                fileName[count] = '\0';

                switch (notify->Action) {
                case FILE_ACTION_ADDED:
                    WriteLog(dir_log_file, "File created: %s\n", fileName);
                    break;
                case FILE_ACTION_REMOVED:
                    WriteLog(dir_log_file, "File deleted: %s\n", fileName);
                    break;
                case FILE_ACTION_MODIFIED:
                    WriteLog(dir_log_file, "File modified: %s\n", fileName);
                    break;
                case FILE_ACTION_RENAMED_OLD_NAME:
                    WriteLog(dir_log_file, "File renamed from: %s\n", fileName);
                    break;
                case FILE_ACTION_RENAMED_NEW_NAME:
                    WriteLog(dir_log_file, "File renamed to: %s\n", fileName);
                    break;
                default:
                    WriteLog(dir_log_file, "Unknown action: %s\n", fileName);
                    break;
                }

                notify = (FILE_NOTIFY_INFORMATION *)((char *)notify + notify->NextEntryOffset);
            } while (notify->NextEntryOffset);
        }
    }

    CloseHandle(hDir);
    WriteLog(srv_log_file, "Service stopped.\n");
}

VOID WINAPI ServiceCtrlHandler(DWORD dwControl) {
    switch (dwControl) {
    case SERVICE_CONTROL_STOP:
        WriteLog(srv_log_file, "Service is stopping...\n");
        service_status.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(hServiceStatus, &service_status);
        SetEvent(hStopEvent);
        service_status.dwCurrentState = SERVICE_STOPPED;
        WriteLog(srv_log_file, "Service stopped successfully.\n");
        break;

    case SERVICE_CONTROL_PAUSE:
        WriteLog(srv_log_file, "Service is pausing...\n");
        service_status.dwCurrentState = SERVICE_PAUSED;
        SetServiceStatus(hServiceStatus, &service_status);
        ResetEvent(hPauseEvent);
        WriteLog(srv_log_file, "Service paused successfully.\n");
        break;

    case SERVICE_CONTROL_CONTINUE:
        WriteLog(srv_log_file, "Service is resuming...\n");
        service_status.dwCurrentState = SERVICE_RUNNING;
        SetServiceStatus(hServiceStatus, &service_status);
        SetEvent(hPauseEvent);
        WriteLog(srv_log_file, "Service resumed successfully.\n");
        break;

    case SERVICE_CONTROL_SHUTDOWN:
        WriteLog(srv_log_file, "Service is shutting down...\n");
        SetEvent(hStopEvent);
        service_status.dwCurrentState = SERVICE_STOPPED;
        WriteLog(srv_log_file, "Service shut down successfully.\n");
        break;

    default:
        WriteLog(srv_log_file, "Received unknown control code: %d\n", dwControl);
        break;
    }

    SetServiceStatus(hServiceStatus, &service_status);
}
