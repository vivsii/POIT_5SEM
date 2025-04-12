#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>

#define MAX_PRIMES 1024
#define FIFO_NAME_TEMPLATE "/tmp/lab03_fifo_%d"

bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <Lower Bound> <Upper Bound> <FIFO Name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);
    const char* fifo_name = argv[3];

    sem_t* mutex = sem_open("/lab03_mutex", O_CREAT, 0644, 1);
    if (mutex == SEM_FAILED) {
        fprintf(stderr, "Error opening/creating semaphore: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    int primes[MAX_PRIMES];
    int count = 0;

    for (int i = lower; i <= upper; i++) {
        if (is_prime(i) && count < MAX_PRIMES) {
            primes[count++] = i;
        }
    }

    int fd = open(fifo_name, O_WRONLY);
    if (fd == -1) {
        fprintf(stderr, "Error opening FIFO for writing: %s\n", strerror(errno));
        sem_close(mutex);
        return EXIT_FAILURE;
    }

    if (write(fd, &count, sizeof(count)) != sizeof(count)) {
        fprintf(stderr, "Error writing count to FIFO\n");
        close(fd);
        sem_close(mutex);
        return EXIT_FAILURE;
    }

    if (write(fd, primes, sizeof(int) * count) != sizeof(int) * count) {
        fprintf(stderr, "Error writing primes to FIFO\n");
    }
    else {
        printf("Client: Found %d primes\n", count);  // Debug message
    }

    close(fd);
    sem_close(mutex);

    return EXIT_SUCCESS;
}