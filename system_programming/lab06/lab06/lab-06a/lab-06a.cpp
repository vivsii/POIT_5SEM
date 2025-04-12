#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <windows.h>

void PrintInfo(const char* FileName) {
    struct stat fileInfo;
    if (stat(FileName, &fileInfo) != 0) {
        std::cerr << "Error: Cant`t get information about file." << std::endl;
        return;
    }

    long long size = fileInfo.st_size;
    std::cout << "File name: " << FileName << std::endl;
    std::cout << "File sizd: " << size << " byte" << std::endl;
    std::cout << "Time created: " << ctime(&fileInfo.st_ctime);
    std::cout << "Last access: " << ctime(&fileInfo.st_atime);
    std::cout << "Last edit: " << ctime(&fileInfo.st_mtime);
}

bool IsTextFile(const char* FileName) {
    std::ifstream file(FileName, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file." << std::endl;
        return false;
    }

    char c;
    while (file.get(c)) {
        if (c < 0 && c != '\n' && c != '\r') {
            return false; // не текстовый
        }
    }
    return true; // текстовый файл
}

void PrintText(const char* FileName) {
    if (!IsTextFile(FileName)) {
        std::cout << "File isn`t txt." << std::endl;
        return;
    }

    std::ifstream file(FileName);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Using: " << argv[0] << " <file_name>" << std::endl;
        return 1;
    }

    const char* fileName = argv[1];
    PrintInfo(fileName);
    PrintText(fileName);

    return 0;
}