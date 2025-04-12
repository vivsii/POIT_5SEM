#include <stdio.h>
#include <unistd.h>
int main() {
    for (int i = 0; i < 50; i++) {
        printf("Child Process (PID: %d) - Iteration: %d\n", getpid(), i + 1);
        sleep(1);
    }
    return 0;
}

