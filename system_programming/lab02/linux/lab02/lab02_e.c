#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;


    pid1 = fork();
    if (pid1 < 0) {
        perror("Failed to fork");
        return 1;
    }
    else if (pid1 == 0) {

        char* args1[] = { "./lab02_x", "20", NULL };
        execvp(args1[0], args1);
        perror("execvp failed");
        exit(1);
    }

    char* envp[] = { "ITER_NUM=30", NULL };

    pid2 = fork();
    if (pid2 < 0) {
        perror("Failed to fork");
        return 1;
    }
    else if (pid2 == 0) {
        execle("./lab02_x", "./lab02_x", NULL, envp);
        perror("execle failed");
        exit(1);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("All child processes have completed.\n");
    return 0;
}
