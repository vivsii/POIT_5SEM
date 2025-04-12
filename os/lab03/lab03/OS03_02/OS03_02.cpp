#include <iostream>
#include <process.h>
#include <windows.h>
using namespace std;
int main(int argc, char* argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL bRes = CreateProcessW(L"OS03_02_1.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, LPSTARTUPINFOW(&si), &pi);
    if (!bRes)
        cout << "Error - " << GetLastError() << endl;

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    STARTUPINFO si2;
    PROCESS_INFORMATION pi2;
    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    ZeroMemory(&pi2, sizeof(pi2));

    BOOL bRes2 = CreateProcessW(L"OS03_02_2.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, LPSTARTUPINFOW(&si2), &pi2);
    if (!bRes2)
        cout << "Error - " << GetLastError() << endl;

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    int iter_number = 100;
    cout << "OS03_02: Number of iterations - " << iter_number << endl;

    for (int i = 0; i < iter_number; i++) {
        cout << "OS03_02: Iteration - " << i + 1 << ", PID: " << GetCurrentProcessId() << endl;
        Sleep(1000);
    }

    return 0;
}


