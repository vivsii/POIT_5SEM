//Lab-06b
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HANDLE hFile;

int GetFileLineCount(HANDLE hFile) {
	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize == INVALID_FILE_SIZE) {
		fprintf(stderr, "Error: Could not get file size. Code: %ld\n", GetLastError());
		return -1;
	}

	DWORD bytesRead;
	char buffer[1];
	int lineCount = 0;
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	while (ReadFile(hFile, buffer, 1, &bytesRead, NULL) && bytesRead > 0) {
		if (buffer[0] == '\n') {
			lineCount++;
		}
	}
	return lineCount;
}

void OpenFilee(LPSTR FileName) {
	hFile = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL) {
		fprintf(stderr, "Error: Could not open file. Code: %ld\n", GetLastError());
		return;
	}
	printf("File opened successfully.\n");
}

void AddRow(LPSTR row, int pos) {
	if (hFile == INVALID_HANDLE_VALUE || row == NULL || hFile == NULL) {
		fprintf(stderr, "Error: Invalid file descriptor or NULL row.\n");
		return;
	}

	DWORD bytesWritten, bytesRead, fileSize = GetFileSize(hFile, NULL);
	BOOL result;
	int lineCount = GetFileLineCount(hFile);

	if (lineCount == -1) return;

	char* buf = (char*)malloc(fileSize + strlen(row) + 2);
	if (!buf) {
		fprintf(stderr, "Error: Memory allocation failed.\n");
		return;
	}

	if (pos == -1) {
		SetFilePointer(hFile, 0, NULL, FILE_END);
		result = WriteFile(hFile, row, strlen(row), &bytesWritten, NULL);
		WriteFile(hFile, "\n", 1, &bytesWritten, NULL);
	}
	else if (pos == 0) {
		SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		ReadFile(hFile, buf + strlen(row) + 1, fileSize, &bytesRead, NULL);
		buf[bytesRead + strlen(row) + 1] = '\0';
		memcpy(buf, row, strlen(row));
		buf[strlen(row)] = '\n';
		SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		result = WriteFile(hFile, buf, fileSize + strlen(row) + 1, &bytesWritten, NULL);
	}
	else if (pos >= 1 && pos <= lineCount + 1) {
		SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		int offset = 0, currentLine = 1;
		ReadFile(hFile, buf + offset, fileSize, &bytesRead, NULL);

		for (int i = 0; i < strlen(buf); i++) {
			if (buf[i] == '\n') {
				currentLine++;
				if (currentLine == pos) break;
			}
			offset++;
		}
		memmove(buf + offset + 2 + strlen(row), buf + offset + 1, fileSize - offset);
		memcpy(buf + offset + 1, row, strlen(row));
		buf[offset + 1 + strlen(row)] = '\n';
		SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		result = WriteFile(hFile, buf, fileSize + strlen(row) + 1, &bytesWritten, NULL);
	}
	else {
		fprintf(stderr, "Error: Invalid position.\n");
		free(buf);
		return;
	}
	free(buf);
	if (!result) {
		fprintf(stderr, "Error: Could not write to file. Code: %ld\n", GetLastError());
	}
}

void RemRow(int pos) {
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL) {
		fprintf(stderr, "Error: Invalid file descriptor.\n");
		return;
	}

	int lineCount = GetFileLineCount(hFile);
	if (lineCount == -1 || pos > lineCount || pos < -1) {
		fprintf(stderr, "Error: Invalid position.\n");
		return;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);
	char* buf = (char*)malloc(fileSize);
	DWORD bytesRead, bytesWritten;
	BOOL result;
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, buf, fileSize, &bytesRead, NULL);

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
		for (int i = 0; i < bytesRead; i++) {
			if (buf[i] == '\n') {
				if (currentLine == pos) start = i + 1;
				if (currentLine == pos + 1) end = i + 1;
				currentLine++;
			}
		}
		memmove(buf + start, buf + end, fileSize - end);
		fileSize -= end - start;
	}

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	result = WriteFile(hFile, buf, fileSize, &bytesWritten, NULL);
	SetEndOfFile(hFile);
	free(buf);
	if (!result) {
		fprintf(stderr, "Error: Could not write to file. Code: %ld\n", GetLastError());
	}
}

void PrintRow(int pos) {
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL) {
		fprintf(stderr, "Error: Invalid file descriptor.\n");
		return;
	}

	int lineCount = GetFileLineCount(hFile);
	if (lineCount == -1 || pos < -1 || pos > lineCount) {
		fprintf(stderr, "Error: Invalid position.\n");
		return;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);
	char* buf = (char*)malloc(fileSize);
	DWORD bytesRead;
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, buf, fileSize, &bytesRead, NULL);

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
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL) {
		fprintf(stderr, "Error: Invalid file descriptor.\n");
		return;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);
	char* buf = (char*)malloc(fileSize);
	DWORD bytesRead;
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, buf, fileSize, &bytesRead, NULL);

	for (DWORD i = 0; i < fileSize; i++) printf("%c", buf[i]);
	printf("\n");
	free(buf);
}

void CloseFile() {
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL) {
		fprintf(stderr, "Error: No file to close.\n");
		return;
	}
	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;
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
			OpenFilee(filePath);
			if (hFile == INVALID_HANDLE_VALUE) {
				printf("Failed to open file.\n");
			}
			else {
				printf("File opened successfully.\n");
			}
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
			if (hFile != INVALID_HANDLE_VALUE) {
				CloseHandle(hFile);
			}
			printf("Exiting program.\n");
			return 0;

		default:
			printf("Invalid choice. Please try again.\n");
		}
	}
}
