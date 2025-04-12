#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

// Глобальные переменные
HANDLE hFileMapping = NULL;
struct Student* pStudents = NULL;
int listSize = 0;

int OpenMapping(LPSTR filePath, LPINT size) {
    HANDLE hFile = CreateFileA(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Не удалось открыть или создать файл: %s\n", filePath);
        return GetLastError();
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        fprintf(stderr, "Не удалось получить размер файла.\n");
        return GetLastError();
    }

    if (fileSize == 0) {
        *size = MAX_STUDENTS;
        struct Student emptyStudent = { 0 };
        for (int i = 0; i < *size; i++) {
            WriteFile(hFile, &emptyStudent, sizeof(struct Student), NULL, NULL);
        }
        WriteFile(hFile, size, sizeof(int), NULL, NULL);
    }
    else {
        ReadFile(hFile, size, sizeof(int), NULL, NULL);
    }

    hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, sizeof(struct Student) * (*size), "StudentMapping");
    if (hFileMapping == NULL) {
        CloseHandle(hFile);
        fprintf(stderr, "Не удалось создать маппинг.\n");
        return GetLastError();
    }

    pStudents = (struct Student*)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(struct Student) * (*size));
    if (pStudents == NULL) {
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        fprintf(stderr, "Не удалось отобразить маппинг в память.\n");
        return GetLastError();
    }

    for (int i = 0; i < *size; i++) {
        if (pStudents[i].ID[0] == '\0') {
            continue;
        }
    }

    CloseHandle(hFile);
    return 0;
}


int AddRow(struct Student row, INT pos) {
    if (hFileMapping == NULL || pStudents == NULL) {
        fprintf(stderr, "Ошибка: маппинг не открыт.\n");
        return -1;
    }

    if (pos < 0) {
        pos += listSize;
    }

    if (pos < 0 || pos >= listSize) {
        fprintf(stderr, "Ошибка: позиция вне диапазона. Введено: %d\n", pos);
        return -2;
    }

    if (pStudents[pos].ID[0] != '\0') {
        fprintf(stderr, "Ошибка: позиция %d занята, выберите другую.\n", pos);
        return -3;
    }

    pStudents[pos] = row;
    return 0;
}


int RemRow(INT lineNumber) {
    if (hFileMapping == NULL || pStudents == NULL) return -1;
    if (lineNumber < 0 || lineNumber >= listSize) {
        fprintf(stderr, "Ошибка: номер строки вне диапазона. Введено: %d\n", lineNumber);
        return -2;
    }

    if (pStudents[lineNumber].ID[0] == '\0') {
        fprintf(stderr, "Ошибка: строка %d пуста, ничего не удалять.\n", lineNumber);
        return -2;
    }

    memset(&pStudents[lineNumber], 0, sizeof(struct Student));
    return 0;
}


int PrintRow(INT pos) {
    if (hFileMapping == NULL || pStudents == NULL) return -1;
    if (pos < 0) pos += listSize;
    if (pos < 0 || pos >= listSize || pStudents[pos].ID[0] == '\0') return -2;

    printf("Имя: %s, Фамилия: %s, Курс: %u, Группа: %u, ID: %s\n",
        pStudents[pos].Name, pStudents[pos].Surname,
        pStudents[pos].Course, pStudents[pos].Group, pStudents[pos].ID);
    return 0;
}


int PrintRows() {
    if (hFileMapping == NULL || pStudents == NULL) return -1;

    for (int i = 0; i < listSize; i++) {
        if (pStudents[i].ID[0] != '\0') {
            PrintRow(i);
        }
    }
    return 0;
}


int CloseMapping() {
    if (hFileMapping == NULL) return -1;

    UnmapViewOfFile(pStudents);
    CloseHandle(hFileMapping);
    hFileMapping = NULL;
    return 0;
}


void displayMenu() {
    printf("1. Открыть файл\n");
    printf("2. Добавить студента\n");
    printf("3. Удалить студента\n");
    printf("4. Показать студента\n");
    printf("5. Показать всех студентов\n");
    printf("6. Закрыть программу\n");
}

int main() {
    int choice;

    while (1) {
        displayMenu();
        printf("Выберите действие: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char filePath[256];
            printf("Введите имя файла: ");
            scanf("%s", filePath);

            if (OpenMapping(filePath, &listSize) != 0) {
                fprintf(stderr, "Ошибка при открытии маппинга\n");
            }
            else {
                printf("Файл успешно открыт.\n");
            }
        }
        else if (choice == 2) {
            struct Student newStudent;
            printf("Введите имя: ");
            scanf("%s", newStudent.Name);
            printf("Введите фамилию: ");
            scanf("%s", newStudent.Surname);
            printf("Введите курс: ");
            scanf("%hhu", &newStudent.Course);
            printf("Введите группу: ");
            scanf("%hhu", &newStudent.Group);
            printf("Введите ID: ");
            scanf("%s", newStudent.ID);

            int position;
            printf("Введите позицию для добавления (отрицательное значение для добавления с конца): ");
            scanf("%d", &position);

            if (AddRow(newStudent, position) != 0) {
                fprintf(stderr, "Ошибка при добавлении студента.\n");
            }
        }
        else if (choice == 3) {
            int pos;
            printf("Введите позицию для удаления: ");
            scanf("%d", &pos);
            if (RemRow(pos) != 0) {
                fprintf(stderr, "Ошибка при удалении студента.\n");
            }
        }
        else if (choice == 4) {
            int pos;
            printf("Введите позицию для показа: ");
            scanf("%d", &pos);
            if (PrintRow(pos) != 0) {
                fprintf(stderr, "Ошибка при показе студента.\n");
            }
        }
        else if (choice == 5) {
            if (PrintRows() != 0) {
                fprintf(stderr, "Ошибка при показе всех студентов.\n");
            }
        }
        else if (choice == 6) {
            CloseMapping();
            break;
        }
        else {
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    }

    return 0;
}