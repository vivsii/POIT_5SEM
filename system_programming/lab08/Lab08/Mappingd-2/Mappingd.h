#ifndef MAPPINGD_H
#define MAPPINGD_H

#include <windows.h>

// Структура для работы с данными
struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

// Прототипы функций
__declspec(dllexport) int OpenMapping(LPSTR filePath, LPINT userListSize);
__declspec(dllexport) int AddRow(struct Student row, INT pos);
__declspec(dllexport) int RemRow(INT pos);
__declspec(dllexport) int PrintRow(INT pos);
__declspec(dllexport) int PrintRows();
__declspec(dllexport) int CloseMapping(HANDLE hFileMapping);
#endif // MAPPINGD_H
