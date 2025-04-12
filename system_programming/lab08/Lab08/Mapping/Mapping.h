#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <windows.h>

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

extern HANDLE hFileMapping;

int OpenMapping(LPSTR filePath, LPINT userListSize);
int AddRow(struct Student row, INT pos);
int RemRow(INT pos);
int PrintRow(INT pos);
int PrintRows();
int CloseMapping(HANDLE hFileMapping);

#endif // MAPPINGS_H
