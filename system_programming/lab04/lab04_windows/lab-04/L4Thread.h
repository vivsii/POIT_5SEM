#ifndef L4THREAD_H
#define L4THREAD_H

#include <Windows.h>

// Прототипы функций для работы с TLS
bool InitializeTLS();
void CleanupTLS();

// Прототип функции потока
DWORD WINAPI L4Thread(LPVOID param);

#endif // L4THREAD_H
