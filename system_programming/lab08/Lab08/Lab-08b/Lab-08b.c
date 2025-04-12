#include <stdio.h>
#include <windows.h>

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

typedef int (*OpenMappingFunc)(LPSTR, LPINT);
typedef int (*AddRowFunc)(struct Student, INT);
typedef int (*RemRowFunc)(INT);
typedef int (*PrintRowFunc)(INT);
typedef int (*PrintRowsFunc)();
typedef void (*CloseMappingFunc)(HANDLE);

OpenMappingFunc OpenMapping = NULL;
AddRowFunc AddRow = NULL;
RemRowFunc RemRow = NULL;
PrintRowFunc PrintRow = NULL;
PrintRowsFunc PrintRows = NULL;
CloseMappingFunc CloseMapping = NULL;

void displayMenu() {
    printf("1. Open mapping\n");
    printf("2. Add row\n");
    printf("3. Remove row\n");
    printf("4. Print row\n");
    printf("5. Print rows\n");
    printf("6. Close mapping\n");
}

HMODULE loadLibraryAndFunctions(const char* dllName) {
    HMODULE hLib = LoadLibrary(dllName);
    if (!hLib) {
        fprintf(stderr, "Error: Failed to load %s\n", dllName);
        return NULL;
    }

    OpenMapping = (OpenMappingFunc)GetProcAddress(hLib, "OpenMapping");
    AddRow = (AddRowFunc)GetProcAddress(hLib, "AddRow");
    RemRow = (RemRowFunc)GetProcAddress(hLib, "RemRow");
    PrintRow = (PrintRowFunc)GetProcAddress(hLib, "PrintRow");
    PrintRows = (PrintRowsFunc)GetProcAddress(hLib, "PrintRows");
    CloseMapping = (CloseMappingFunc)GetProcAddress(hLib, (LPCSTR)6); // Импорт по порядковому номеру

    if (!OpenMapping || !AddRow || !RemRow || !PrintRow || !PrintRows || !CloseMapping) {
        fprintf(stderr, "Error: Failed to get function addresses from %s\n", dllName);
        FreeLibrary(hLib);
        return NULL;
    }

    return hLib;
}

int main() {
    int choice;
    int listSize = 0;
    HANDLE hFileMapping = NULL;

    HMODULE hMappingd = loadLibraryAndFunctions("libMappingd.dll");
    if (!hMappingd) return -1;
/*
    HMODULE hMappingd2 = loadLibraryAndFunctions("libMappingd-2.dll");
    if (!hMappingd2) {
        FreeLibrary(hMappingd);
        return -1;
    }
*/
    // Основной цикл
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
                } else {
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
                printf("Enter year: ");
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
                } else {
                    printf("Student added successfully.\n");
                }
                break;
            }
            case 3: {
                int pos;
                printf("Enter index of the row to delete: ");
                scanf("%d", &pos);
                if (RemRow(pos) != 0) {
                    fprintf(stderr, "Error while removing.\n");
                } else {
                    printf("Row removed successfully.\n");
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
                CloseMapping(hFileMapping);
                printf("Mapping closed.\n");
                break;
            }
            default: {
                printf("Incorrect input. Try again.\n");
                break;
            }
        }
    }

    // Освобождение библиотек
    FreeLibrary(hMappingd);
    //FreeLibrary(hMappingd2);

    return 0;
}
