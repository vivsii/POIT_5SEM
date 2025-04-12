#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

#define MAX_STUDENTS 100

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

void* pMappedView = NULL;
int listSize = 0;
int fd = -1;

int OpenMapping(const char* filePath, int* listSize) {
    fd = open(filePath, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == 0) {
        fileSize = (*listSize) * sizeof(struct Student);
        if (ftruncate(fd, fileSize) == -1) {
            perror("Error setting file size");
            close(fd);
            return -1;
        }
    }

    *listSize = fileSize / sizeof(struct Student);
    pMappedView = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pMappedView == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return -1;
    }

    printf("File mapping successfully created.\n");
    return 0;
}

int AddRow(struct Student row, int pos) {
    if (!pMappedView) return -1;

    struct Student* students = (struct Student*)pMappedView;
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

    struct Student* students = (struct Student*)pMappedView;
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

    struct Student* students = (struct Student*)pMappedView;
    if (pos < 0) pos = listSize + pos;

    if (pos < 0 || pos >= listSize || students[pos].Name[0] == '\0') {
        printf("Error: Invalid position or position empty.\n");
        return -1;
    }

    printf("Student at position %d:\nName: %s\nSurname: %s\nCourse: %u\nGroup: %u\nID: %s\n",
        pos, students[pos].Name, students[pos].Surname, students[pos].Course, students[pos].Group, students[pos].ID);
    return 0;
}

int PrintRows() {
    if (!pMappedView) return -1;

    struct Student* students = (struct Student*)pMappedView;
    for (int i = 0; i < listSize; i++) {
        if (students[i].Name[0] != '\0') {
            printf("Position %d:\nName: %s\nSurname: %s\nCourse: %u\nGroup: %u\nID: %s\n\n",
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

    if (fd != -1) {
        close(fd);
        fd = -1;
    }

    printf("Mapping closed successfully.\n");
    return 0;
}

void displayMenu() {
    printf("1. Open mapping\n");
    printf("2. Add row\n");
    printf("3. Remove row\n");
    printf("4. Print row\n");
    printf("5. Print rows\n");
    printf("6. Close mapping\n");
}

int main() {
    int choice;

    while (1) {
        displayMenu();
        printf("Choose option: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            char filePath[256];
            printf("Enter path to file: ");
            scanf("%s", filePath);

            if (OpenMapping(filePath, &listSize) != 0) {
                fprintf(stderr, "Error opening file\n");
            }
            else {
                printf("File opened.\n");
            }
            break;
        }
        case 2: {
            struct Student newStudent;
            printf("Enter name: ");
            scanf("%s", newStudent.Name);
            printf("Enter surname: ");
            scanf("%s", newStudent.Surname);
            printf("Enter course: ");
            scanf("%hhu", &newStudent.Course);
            printf("Enter group: ");
            scanf("%hhu", &newStudent.Group);
            printf("Enter ID: ");
            scanf("%s", newStudent.ID);

            int position;
            printf("Enter index of the row to add (negative values for add from end): ");
            scanf("%d", &position);

            if (AddRow(newStudent, position) != 0) {
                fprintf(stderr, "Error while adding.\n");
            }
            break;
        }
        case 3: {
            int pos;
            printf("Enter index of the row to delete: ");
            scanf("%d", &pos);
            if (RemRow(pos) != 0) {
                fprintf(stderr, "Error while removing.\n");
            }
            break;
        }
        case 4: {
            int pos;
            printf("Enter index of the row to print: ");
            scanf("%d", &pos);
            if (PrintRow(pos) != 0) {
                fprintf(stderr, "Error while printing one row.\n");
            }
            break;
        }
        case 5: {
            if (PrintRows() != 0) {
                fprintf(stderr, "Error while printing all rows.\n");
            }
            break;
        }
        case 6: {
            CloseMapping();
            break;
        }
        default: {
            printf("Incorrect input. Try again.\n");
            break;
        }
        }
    }

    return 0;
}
