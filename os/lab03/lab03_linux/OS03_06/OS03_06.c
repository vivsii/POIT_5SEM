#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{

    system("./OS03_05_1");

    int iter_number = 100;

    printf("Iterations number: %d\n", iter_number);
    for (int i = 0; i < iter_number; i++) {
        printf("Iteration - %d, PID: %d \n", i + 1, getpid());
        sleep(5);
    }

    return 0;
}
