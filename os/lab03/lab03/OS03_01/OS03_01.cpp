#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
using namespace std;
int main(int argc, char* argv[])
{
    int iter_number = 0;
    cout << "Enter number of iterations \n";
    cin >> iter_number;

    if (iter_number <= 0) {
        cout << "Error: The number of iterations must be a positive number.\n";
        ExitProcess(1);
    }
    for (int i = 0; i < iter_number; i++) {
        cout << "Iteration - " << i + 1 << ", PID: " << GetCurrentProcessId() << endl;
        Sleep(5000);
    }

    return 0;
}
