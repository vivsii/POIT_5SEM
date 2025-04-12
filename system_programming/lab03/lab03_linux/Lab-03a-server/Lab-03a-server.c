#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#define FIFO_TEMPLATE "/tmp/lab03_fifo_%d"
#define FIFO_NAME_MAX 256

void create_child_process(int lower, int upper, int process_id, const char* mutex_name) {
    char fifo_name[FIFO_NAME_MAX];
    snprintf(fifo_name, FIFO_NAME_MAX, FIFO_TEMPLATE, process_id);

    if (mkfifo(fifo_name, 0666) == -1 && errno != EEXIST) {
        fprintf(stderr, "Error creating FIFO: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    char lower_str[32], upper_str[32];
    snprintf(lower_str, sizeof(lower_str), "%d", lower);
    snprintf(upper_str, sizeof(upper_str), "%d", upper);

    printf("Starting client with bounds [%d, %d] using FIFO %s\n", lower, upper, fifo_name); // Debug message

    execl("./Lab-03a-client", "Lab-03a-client", lower_str, upper_str, fifo_name, (char*)NULL);
    fprintf(stderr, "Error executing child process: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        fprintf(stderr, "Usage: %s <Num Processes> <Lower Bound> <Upper Bound> [Mutex Name]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_processes = atoi(argv[1]);
    int lower = atoi(argv[2]);
    int upper = atoi(argv[3]);
    const char* mutex_name = (argc == 5) ? argv[4] : "/lab03_mutex";

    if (lower > upper) {
        int temp = lower;
        lower = upper;
        upper = temp;
    }

    sem_t* mutex = sem_open(mutex_name, O_CREAT, 0644, 1);
    if (mutex == SEM_FAILED) {
        fprintf(stderr, "Error creating semaphore: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    int range = (upper - lower + 1) / num_processes;

    for (int i = 0; i < num_processes; i++) {
        int child_lower = lower + i * range;
        int child_upper = (i == num_processes - 1) ? upper : (child_lower + range - 1);

        pid_t pid = fork();
        if (pid == 0) {
            create_child_process(child_lower, child_upper, i, mutex_name);
        }
        else if (pid < 0) {
            fprintf(stderr, "Error forking child process: %s\n", strerror(errno));
            sem_close(mutex);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < num_processes; i++) {
        char fifo_name[FIFO_NAME_MAX];
        snprintf(fifo_name, FIFO_NAME_MAX, FIFO_TEMPLATE, i);

        int fd = open(fifo_name, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "Error opening FIFO for reading: %s\n", strerror(errno));
            continue;
        }

        int count = 0;
        if (read(fd, &count, sizeof(count)) != sizeof(count)) {
            fprintf(stderr, "Error reading count from FIFO\n");
            close(fd);
            continue;
        }

        printf("Process %d found %d primes:\n", i + 1, count);

        if (count > 0) {
            int* primes = malloc(sizeof(int) * count);
            if (read(fd, primes, sizeof(int) * count) != sizeof(int) * count) {
                fprintf(stderr, "Error reading primes from FIFO\n");
                free(primes);
                close(fd);
                continue;
            }

            for (int j = 0; j < count; j++) {
                printf("%d ", primes[j]);
            }
            printf("\n");
            free(primes);
        }

        close(fd);
        unlink(fifo_name);
    }

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    sem_close(mutex);
    sem_unlink(mutex_name);

    return EXIT_SUCCESS;
}