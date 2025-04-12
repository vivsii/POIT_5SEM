#include <iostream>
#include <windows.h>
using namespace std;
int main(int argc, char* argv[])
{
    int iter_number = 10;
    cout << "OS03_02_2: Number of iterations - " << iter_number << endl;

    for (int i = 0; i < iter_number; i++) {
        cout << "OS03_02_2: Iteration - " << i + 1 << ", PID: " << GetCurrentProcessId() << endl;
        Sleep(1000);
    }

    return 0;
}


