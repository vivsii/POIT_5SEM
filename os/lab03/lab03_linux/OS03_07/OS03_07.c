#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
    if (fork() == 0) {
        execl("./OS03_05_1_7", "OS03_05_1_7", NULL);
        perror("execl failed");
        return 1;
    }


    int iter_number = 100;

    printf("Iterations number: %d\n", iter_number);
    for (int i = 0; i < iter_number; i++) {
        printf("Iteration - %d, PID: %d \n", i + 1, getpid());
        sleep(5);
    }

    return 0;
}
