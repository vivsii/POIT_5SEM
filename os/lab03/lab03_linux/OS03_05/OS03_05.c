#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_process() {
    for (int i = 0; i < 50; i++) {
        printf("Child (PID: % d) - : % d\n", getpid(), i + 1);
        sleep(1);
    }
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Îøèáêà fork");
        return 1;
    }
    else if (pid == 0) {
        child_process();
        return 0;
    }
    else {
        printf("(PID: %d)\n", getpid());
        for (int i = 0; i < 100; i++) {
            printf("Parent (PID: %d) - : %d\n", getpid(), i + 1);
            sleep(1);
        }

        wait(NULL);
        printf("Child done.\n");
    }

    return 0;
}