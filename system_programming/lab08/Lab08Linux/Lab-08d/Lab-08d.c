#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

size_t ListSize = 0;

void displayMenu() {
    printf("1. Open mapping\n");
    printf("2. Add row\n");
    printf("3. Remove row\n");
    printf("4. Print row\n");
    printf("5. Print rows\n");
    printf("6. Close mapping\n");
}

int main() {

    void* handle = dlopen("./libMappingd.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Cannot open library: %s\n", dlerror());
        return 1;
    }

    int (*OpenMapping)(const char*, size_t) = dlsym(handle, "OpenMapping");
    int (*AddRow)(struct Student, int) = dlsym(handle, "AddRow");
    int (*RemRow)(int) = dlsym(handle, "RemRow");
    int (*PrintRow)(int) = dlsym(handle, "PrintRow");
    int (*PrintRows)() = dlsym(handle, "PrintRows");
    int (*CloseMapping)() = dlsym(handle, "CloseMapping");


    if (!OpenMapping || !AddRow || !RemRow || !PrintRow || !PrintRows || !CloseMapping) {
        fprintf(stderr, "Cannot load symbols: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    struct Student newStudent;
    int choice;

    while (1) {
        displayMenu();
        printf("Select an action: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char filePath[256];
            printf("Enter file name: ");
            scanf("%s", filePath);
            OpenMapping(filePath, &ListSize);
        }
        else if (choice == 2) {
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

            AddRow(newStudent, position);
        }
        else if (choice == 3) {
            int pos;
            printf("Enter position to remove: ");
            scanf("%d", &pos);
            RemRow(pos);
        }
        else if (choice == 4) {
            int pos;
            printf("Enter position to show: ");
            scanf("%d", &pos);
            PrintRow(pos);
        }
        else if (choice == 5) {
            PrintRows();
        }
        else if (choice == 6) {
            break;
        }
        else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    dlclose(handle);
    return 0;
}