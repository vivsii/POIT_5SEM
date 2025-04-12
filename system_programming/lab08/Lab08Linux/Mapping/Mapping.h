#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <stdio.h>

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

int OpenMapping(const char* filePath, int* userListSize);
int AddRow(struct Student row, int pos);
int RemRow(int pos);
int PrintRow(int pos);
int PrintRows();
int CloseMapping();

#endif // MAPPINGS_H
