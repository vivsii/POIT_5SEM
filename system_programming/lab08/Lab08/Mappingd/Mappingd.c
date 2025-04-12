#include "Mappingd.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HANDLE hFileMapping = NULL;
LPVOID pMappedView = NULL;
int listSize = 0;

int OpenMapping(LPSTR filePath, LPINT userListSize) {
    HANDLE hFile = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Error: Unable to open or create file.\n");
        return -1;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == 0 && *userListSize > 0) {
        fileSize = (*userListSize) * sizeof(struct Student);
        SetFilePointer(hFile, fileSize, NULL, FILE_BEGIN);
        SetEndOfFile(hFile);
    }

    hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, filePath);
    CloseHandle(hFile);

    if (!hFileMapping) {
        printf("Error: Unable to create file mapping.\n");
        return -1;
    }

    pMappedView = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!pMappedView) {
        printf("Error: Unable to map view of file.\n");
        CloseHandle(hFileMapping);
        return -1;
    }

    listSize = fileSize / sizeof(struct Student); // Обновляем глобальный размер списка
    printf("File mapping successfully created.\n");
    return 0;
}

int AddRow(struct Student row, INT pos) {
    if (!pMappedView) return -1;

    struct Student* students = pMappedView;
    if (pos < 0) pos = listSize + pos;

    if (pos < 0 || pos >= listSize || students[pos].Name[0] != '\0') {
        printf("Error: Invalid position or position already occupied.\n");
        return -1;
    }

    students[pos] = row;
    printf("Student added at position %d.\n", pos);
    return 0;
}

int RemRow(INT pos) {
    if (!pMappedView) return -1;

    struct Student* students = pMappedView;
    if (pos < 0) pos = listSize + pos;

    if (pos < 0 || pos >= listSize || students[pos].Name[0] == '\0') {
        printf("Error: Invalid position or position empty.\n");
        return -1;
    }

    memset(&students[pos], 0, sizeof(struct Student));
    printf("Student removed from position %d.\n", pos);
    return 0;
}

int PrintRow(INT pos) {
    if (!pMappedView) return -1;

    struct Student* students = pMappedView;
    if (pos < 0) pos = listSize + pos;

    if (pos < 0 || pos >= listSize || students[pos].Name[0] == '\0') {
        printf("Error: Invalid position or position empty.\n");
        return -1;
    }

    printf("Student at position %d:\nName: %s\nSurname: %s\nYear: %u\nGroup: %u\nID: %s\n",
           pos, students[pos].Name, students[pos].Surname, students[pos].Course, students[pos].Group, students[pos].ID);
    return 0;
}

int PrintRows() {
    if (!pMappedView) return -1;

    struct Student* students = pMappedView;
    for (int i = 0; i < listSize; i++) {
        if (students[i].Name[0] != '\0') {
            printf("Position %d:\nName: %s\nSurname: %s\nYear: %u\nGroup: %u\nID: %s\n\n",
                   i, students[i].Name, students[i].Surname, students[i].Course, students[i].Group, students[i].ID);
        }
    }
    return 0;
}

int CloseMapping(HANDLE hFileMapping) {
    if (pMappedView) {
        UnmapViewOfFile(pMappedView);
        pMappedView = NULL;
    }

    if (hFileMapping) {
        CloseHandle(hFileMapping);
        hFileMapping = NULL;
    }

    listSize = 0;
    printf("Mapping closed successfully.\n");
    return 0;
}
