#ifndef MAPPINGD_H
#define MAPPINGD_H

#include <sys/types.h>
#include <stddef.h>

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};


int OpenMapping(const char* filePath, size_t* listSize);
int AddRow(struct Student student, int position);
int RemRow(int index);
int PrintRow(int index);
int PrintRows();
int CloseMapping();


#endif // MAPPINGD_H
