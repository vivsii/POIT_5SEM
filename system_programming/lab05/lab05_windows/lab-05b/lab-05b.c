#include <stdio.h>
#include <windows.h>
int main()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    DWORD pageSize = si.dwPageSize;
    DWORD numPages = 256;
    DWORD halfPages = numPages / 2;
    DWORD allocSize = numPages * pageSize;

    printf("Page size: %lu bytes\n", pageSize);

    LPVOID baseAddress = VirtualAlloc(NULL, allocSize, MEM_RESERVE, PAGE_READWRITE);
    if (baseAddress == NULL) {
        printf("Error: %lu\n", GetLastError());
        return 1;
    }
    printf("\n 256 pages Base addres: %p\n", baseAddress);
    getchar();

    LPVOID halfAddress = VirtualAlloc(baseAddress, halfPages * pageSize, MEM_COMMIT, PAGE_READWRITE);
    if (baseAddress == NULL) {
        printf("Error: %lu\n", GetLastError());
        return 1;
    }
    printf("\n 256 pages Base addres: %p\n", halfAddress);
    getchar();

    int* intArray = (int*)halfAddress;
    for (int i = 0; i < (halfPages * pageSize) / sizeof(int); i++) {
        intArray[i] = i;
    }
    printf("\n Fill halp virt mem\n");
    getchar();

    DWORD oldProtect;
    if (!VirtualProtect(baseAddress, halfPages * pageSize, PAGE_READONLY, &oldProtect)) {
        printf("Error protect: %lu\n", GetLastError());
        VirtualFree(baseAddress, 0, MEM_RELEASE);
        return 1;
    }
    printf("alter attr succ.\n");
    getchar();

    if (!VirtualFree(halfAddress, halfPages * pageSize, MEM_DECOMMIT)) {
        printf("Error freeing committed memory: %lu\n", GetLastError());
        VirtualFree(baseAddress, 0, MEM_RELEASE);
        return 1;
    }
    printf("Physical memory for second half (128 pages) freed.\n");
    getchar();

    if (!VirtualFree(baseAddress, 0, MEM_RELEASE)) {
        printf("Error releasing virtual memory: %lu\n", GetLastError());
        return 1;
    }
    printf("Virtual memory (256 pages) successfully released.\n");

    return 0;

    return 0;
}
