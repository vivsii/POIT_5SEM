//Lab-06e
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int hFile;

int GetFileLineCount(int hFile) {
    struct stat fileStat;
    if (fstat(hFile, &fileStat) == -1) {
        perror("Error: Could not get file size");
        return -1;
    }

    off_t fileSize = fileStat.st_size;
    char buffer;
    int lineCount = 0;
    lseek(hFile, 0, SEEK_SET); // в начал

    while (read(hFile, &buffer, 1) > 0) {
        if (buffer == '\n') {
            lineCount++;
        }
    }

    lseek(hFile, 0, SEEK_SET);
    return lineCount;
}

void OpenFile(const char* fileName) {
    hFile = open(fileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (hFile == -1) {
        perror("Error: Could not open file");
        return;
    }
    printf("File opened successfully.\n");
}

void AddRow(const char* row, int pos) {
    if (hFile == -1 || row == NULL) {
        fprintf(stderr, "Error: Invalid file descriptor or NULL row.\n");
        return;
    }

    int lineCount = GetFileLineCount(hFile);
    if (lineCount == -1) return;

    // Создаем буфер для хранения данных
    off_t fileSize = lseek(hFile, 0, SEEK_END);
    char* buf = malloc(fileSize + strlen(row) + 2);
    if (!buf) {
        perror("Error: Memory allocation failed");
        return;
    }

    lseek(hFile, 0, SEEK_SET); // в начало
    read(hFile, buf, fileSize);
    buf[fileSize] = '\0';

    if (pos == -1) {
        strcat(buf, row);
        strcat(buf, "\n");
    }
    else if (pos == 0) {
        memmove(buf + strlen(row) + 1, buf, fileSize);
        memcpy(buf, row, strlen(row));
        buf[strlen(row)] = '\n';
    }
    else if (pos >= 1 && pos <= lineCount + 1) {
        int offset = 0, currentLine = 1;
        while (offset < fileSize) {
            if (buf[offset] == '\n') {
                currentLine++;
                if (currentLine == pos) break;
            }
            offset++;
        }
        memmove(buf + offset + strlen(row) + 1, buf + offset, fileSize - offset);
        memcpy(buf + offset, row, strlen(row));
        buf[offset + strlen(row)] = '\n';
    }
    else {
        fprintf(stderr, "Error: Invalid position.\n");
        free(buf);
        return;
    }

    // Записываем обновленный буфер в файл
    lseek(hFile, 0, SEEK_SET);
    write(hFile, buf, strlen(buf));
    ftruncate(hFile, strlen(buf)); // Обрезаем файл по новой длине
    free(buf);
}

void RemRow(int pos) {
    if (hFile == -1) {
        fprintf(stderr, "Error: Invalid file descriptor.\n");
        return;
    }

    int lineCount = GetFileLineCount(hFile);
    if (lineCount == -1 || pos > lineCount || pos < -1) {
        fprintf(stderr, "Error: Invalid position.\n");
        return;
    }

    off_t fileSize = lseek(hFile, 0, SEEK_END);
    char* buf = malloc(fileSize);
    lseek(hFile, 0, SEEK_SET);
    read(hFile, buf, fileSize);

    if (pos == -1) {
        int i = fileSize - 1;
        while (i >= 0 && buf[i] != '\n') i--;
        fileSize = i;
    }
    else if (pos == 0) {
        int i = 0;
        while (i < fileSize && buf[i] != '\n') i++;
        memmove(buf, buf + i + 1, fileSize - i - 1);
        fileSize -= i + 1;
    }
    else {
        int currentLine = 1, start = 0, end = 0;
        for (int i = 0; i < fileSize; i++) {
            if (buf[i] == '\n') {
                if (currentLine == pos) start = i + 1;
                if (currentLine == pos + 1) end = i + 1;
                currentLine++;
            }
        }
        memmove(buf + start, buf + end, fileSize - end);
        fileSize -= end - start;
    }

    lseek(hFile, 0, SEEK_SET);
    write(hFile, buf, fileSize);
    ftruncate(hFile, fileSize);
    free(buf);
}

void PrintRow(int pos) {
    if (hFile == -1) {
        fprintf(stderr, "Error: Invalid file descriptor.\n");
        return;
    }

    int lineCount = GetFileLineCount(hFile);
    if (lineCount == -1 || pos < -1 || pos > lineCount) {
        fprintf(stderr, "Error: Invalid position.\n");
        return;
    }

    off_t fileSize = lseek(hFile, 0, SEEK_END);
    char* buf = malloc(fileSize);
    lseek(hFile, 0, SEEK_SET);
    read(hFile, buf, fileSize);

    if (pos == -1) pos = lineCount;
    int currentLine = 1, i = 0;
    while (i < fileSize) {
        if (currentLine == pos) {
            while (i < fileSize && buf[i] != '\n') printf("%c", buf[i++]);
            printf("\n");
            break;
        }
        if (buf[i++] == '\n') currentLine++;
    }
    free(buf);
}

void PrintRows() {
    if (hFile == -1) {
        fprintf(stderr, "Error: Invalid file descriptor.\n");
        return;
    }

    off_t fileSize = lseek(hFile, 0, SEEK_END);
    char* buf = malloc(fileSize);
    lseek(hFile, 0, SEEK_SET);
    read(hFile, buf, fileSize);

    for (off_t i = 0; i < fileSize; i++) printf("%c", buf[i]);
    printf("\n");
    free(buf);
}

void CloseFile() {
    if (hFile == -1) {
        fprintf(stderr, "Error: No file to close.\n");
        return;
    }
    close(hFile);
    hFile = -1;
    printf("File closed.\n");
}

int main() {
    int choice;
    char filePath[256];
    char row[256];
    int pos;

    while (1) {
        printf("Menu:\n");
        printf("1. Open file\n");
        printf("2. Add row\n");
        printf("3. Remove row\n");
        printf("4. Print row\n");
        printf("5. Print all rows\n");
        printf("6. Close file\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter the file path: ");
            scanf("%s", filePath);
            OpenFile(filePath);
            break;

        case 2:
            printf("Enter the row content: ");
            scanf(" %[^\n]s", row);
            printf("Enter position (0 - beginning, -1 - end, >0 - specific position): ");
            scanf("%d", &pos);
            AddRow(row, pos);
            break;

        case 3:
            printf("Enter position to remove (0 - beginning, -1 - end, >0 - specific position): ");
            scanf("%d", &pos);
            RemRow(pos);
            break;

        case 4:
            printf("Enter row position to print: ");
            scanf("%d", &pos);
            PrintRow(pos);
            break;

        case 5:
            PrintRows();
            break;

        case 6:
            CloseFile();
            break;

        case 0:
            if (hFile != -1) {
                CloseFile();
            }
            printf("Exiting program.\n");
            return 0;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}
