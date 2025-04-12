#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <cstdio>

void out_process(void) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE)
        return;

    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(pe);

    if (!Process32First(snap, &pe)) {
        CloseHandle(snap);
        return;
    }

    do {
        if (pe.th32ProcessID != 0)
            _putts(pe.szExeFile);
    } while (Process32Next(snap, &pe));
    CloseHandle(snap);
}

int main(void) {
    out_process();
    _gettchar();
    return 0;
}