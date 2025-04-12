//Lab-06d
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>

void PrintInfo(const char* FileName);
void PrintText(const char* FileName);

bool IsTextFile(const char* FileName);
void PrintFileTime(struct stat fileStat);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: Lab-06d <file_path>\n");
        return 1;
    }

    const char* fileName = argv[1];

    PrintInfo(fileName);
    PrintText(fileName);

    return 0;
}

void PrintInfo(const char* FileName) {
    struct stat fileStat;
    if (stat(FileName, &fileStat) == -1) {
        perror("Failed to get file attributes");
        return;
    }

    printf("File Information:\n");
    printf("Name: %s\n", FileName);

    printf("Size: %ld B, %.2f KiB, %.2f MiB\n",
        fileStat.st_size,
        fileStat.st_size / 1024.0,
        fileStat.st_size / (1024.0 * 1024.0));

    printf("Type: %s\n",
        S_ISDIR(fileStat.st_mode) ? "Directory" : "File");

    printf("Creation Time: ");
    PrintFileTime(fileStat);

    // Определяем бинарный файл
    bool isBinary = !IsTextFile(FileName);
    printf("Binary File: %s\n", isBinary ? "Yes" : "No");
}

void PrintText(const char* FileName) {
    if (!IsTextFile(FileName)) {
        fprintf(stderr, "The specified file is not a text file.\n");
        return;
    }

    FILE* file = fopen(FileName, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    printf("\nFile Content:\n");
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

bool IsTextFile(const char* FileName) {
    FILE* file = fopen(FileName, "rb");
    if (!file) {
        perror("Failed to open file");
        return false;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch > 127) { // Non-ASCII character found
            fclose(file);
            return false;
        }
    }

    fclose(file);
    return true;
}

void PrintFileTime(struct stat fileStat) {
    char timeStr[100];
    struct tm* timeInfo;

    timeInfo = localtime(&fileStat.st_ctime);
    strftime(timeStr, sizeof(timeStr), "%d/%m/%Y %H:%M:%S", timeInfo);
    printf("%s\n", timeStr);
}
