#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "mapping.h"

void *pMappedView = NULL;
int listSize = 0;
int fileDescriptor = -1;

int OpenMapping(const char *filePath, int *userListSize) {
    fileDescriptor = open(filePath, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fileDescriptor == -1) {
        perror("Error opening file");
        return -1;
    }

    off_t fileSize = lseek(fileDescriptor, 0, SEEK_END);

    if (fileSize == 0 && *userListSize > 0) {
        fileSize = (*userListSize) * sizeof(struct Student);
        if (ftruncate(fileDescriptor, fileSize) == -1) {
            perror("Error resizing file");
            close(fileDescriptor);
            return -1;
        }
    }

    pMappedView = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    if (pMappedView == MAP_FAILED) {
        perror("Error mapping file");
        close(fileDescriptor);
        return -1;
    }

    listSize = fileSize / sizeof(struct Student);
    printf("File mapping successfully created.\n");

    return 0;
}

int AddRow(struct Student row, int pos) {
    if (!pMappedView) return -1;

    struct Student *students = (struct Student *)pMappedView;

    if (pos < 0) pos = listSize + pos;

    if (pos < 0 || pos >= listSize || students[pos].Name[0] != '\0') {
        printf("Error: Invalid position or position already occupied.\n");
        return -1;
    }

    students[pos] = row;
    printf("Student added at position %d.\n", pos);
    return 0;
}

int RemRow(int pos) {
    if (!pMappedView) return -1;

    struct Student *students = (struct Student *)pMappedView;

    if (pos < 0) pos = listSize + pos;

    if (pos < 0 || pos >= listSize || students[pos].Name[0] == '\0') {
        printf("Error: Invalid position or position empty.\n");
        return -1;
    }

    memset(&students[pos], 0, sizeof(struct Student));
    printf("Student removed from position %d.\n", pos);
    return 0;
}

int PrintRow(int pos) {
    if (!pMappedView) return -1;

    struct Student *students = (struct Student *)pMappedView;

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

    struct Student *students = (struct Student *)pMappedView;
    for (int i = 0; i < listSize; i++) {
        if (students[i].Name[0] != '\0') {
            printf("Position %d:\nName: %s\nSurname: %s\nYear: %u\nGroup: %u\nID: %s\n\n",
                   i, students[i].Name, students[i].Surname, students[i].Course, students[i].Group, students[i].ID);
        }
    }
    return 0;
}

int CloseMapping() {
    if (pMappedView) {
        munmap(pMappedView, listSize * sizeof(struct Student));
        pMappedView = NULL;
    }

    if (fileDescriptor != -1) {
        close(fileDescriptor);
        fileDescriptor = -1;
    }

    listSize = 0;
    printf("Mapping closed successfully.\n");
    return 0;
}
