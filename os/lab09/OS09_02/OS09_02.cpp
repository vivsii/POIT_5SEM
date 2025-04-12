#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>

using namespace std;

#define FILE_PATH L"C:\\уник\\os\\lab09\\OS09_02.txt"
#define READ_BYTES 500

void printWideText(const wchar_t* text)
{
    wcout << text << endl;
}

BOOL printFileText(LPWSTR fileName)
{
    try
    {
        SetConsoleOutputCP(CP_UTF8);
        wcout << L"\n\n\t======  RESULT  ======\n";
        HANDLE hf = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hf == INVALID_HANDLE_VALUE) throw "[ERROR] Create or open file failed.";

        DWORD n = NULL;
        char buf[READ_BYTES + 1];
        ZeroMemory(buf, sizeof(buf));

        BOOL b = ReadFile(hf, buf, READ_BYTES, &n, NULL);
        if (!b) throw "[ERROR] Read file failed";

        buf[n] = '\0'; 

        wchar_t wbuf[1024];
        int len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, 1024);
        if (len == 0) throw "[ERROR] MultiByteToWideChar failed";

        printWideText(wbuf);
        CloseHandle(hf);
        return true;
    }
    catch (const char* em)
    {
        cerr << em << endl;
        return false;
    }
}

BOOL delRowFileTxt(LPWSTR fileName, DWORD row)
{
    try
    {
        SetConsoleOutputCP(CP_UTF8);
        wcout << L"\n======  Delete row: " << row << L"\n\n";

        HANDLE hf = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hf == INVALID_HANDLE_VALUE)
        {
            CloseHandle(hf);
            throw "[ERROR] Create or open file failed";
        }

        DWORD n = NULL;
        char buf[1024];
        ZeroMemory(buf, sizeof(buf));

        BOOL b = ReadFile(hf, buf, sizeof(buf), &n, NULL);
        if (!b)
        {
            CloseHandle(hf);
            throw "[ERROR] Read file unsuccessful";
        }
        CloseHandle(hf);

        buf[n] = '\0';

        wchar_t wbuf[1024];
        int len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, 1024);
        if (len == 0) throw "[ERROR] MultiByteToWideChar failed";

        wcout << L"\t\tBEFORE:\n";
        printWideText(wbuf);
        
        wchar_t editedBuf[1024];
        ZeroMemory(editedBuf, sizeof(editedBuf));

        int line = 1;
        int j = 0;
        bool rowFound = false;

        for (int i = 0; wbuf[i] != L'\0'; i++)
        {
            if (line == row)
                rowFound = true;
            else
                editedBuf[j++] = wbuf[i];

            if (wbuf[i] == L'\n')
                line++;
        }

        if (!rowFound) throw "[ERROR] Can't find this row.";

        char outBuf[1024];
        int outLen = WideCharToMultiByte(CP_UTF8, 0, editedBuf, -1, outBuf, 1024, NULL, NULL);
        if (outLen == 0) throw "[ERROR] WideCharToMultiByte failed";

        HANDLE hAppend = CreateFile(fileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hAppend == INVALID_HANDLE_VALUE) throw "[ERROR] Cannot open file for writing";

        b = WriteFile(hAppend, outBuf, outLen - 1, &n, NULL);
        if (!b) throw "[ERROR] Write file unsuccessful";

        CloseHandle(hAppend);

        wcout << L"\n\t\tAFTER:\n";
        printWideText(editedBuf);
        wcout << L"\n==========================================\n";
        return true;
    }
    catch (const char* em)
    {
        cerr << em << endl;
        return false;
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    SetConsoleOutputCP(CP_UTF8);

    LPWSTR file = (LPWSTR)FILE_PATH;

    delRowFileTxt(file, 1);
    delRowFileTxt(file, 3);
    delRowFileTxt(file, 8);
    delRowFileTxt(file, 10);

    printFileText(file);

    return 0;
}
