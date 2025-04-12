#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
int main(int argc, char* argv[])
{
    printf("lab02_x :\n");

    int iter_number = 0;

    if (argc > 1) {
        iter_number = atoi(argv[1]);
    }
    else {
        char* iterEnv = getenv("ITER_NUM");
        if (iterEnv != NULL) {
            iter_number = atoi(iterEnv);
        }
        else {
            printf("Error: Enter number of iterations through terminal arguments or environment variable ITER_NUM");
            return 1;
        }
    }
    printf("Iterations number: %d\n", iter_number);
    if (iter_number <= 0) {
        printf("Error: The number of iterations must be a positive number.\n");
        ExitProcess(1);
    }
    for (int i = 0; i < iter_number; i++) {
        printf("Iteration - %d, PID: %d \n", i + 1, GetCurrentProcessId());
        Sleep(500);
    }

    return 0;
}
