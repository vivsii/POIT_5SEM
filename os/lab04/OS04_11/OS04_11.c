#include <stdio.h>
#include <pthread.h>   // Для работы с потоками
#include <unistd.h>    // Для функции sleep()
#include <sys/types.h> // Для получения идентификатора процесса (pid_t)

// Функция, которая будет выполняться в потоке
void* OS04_11_T1(void* arg) {
    pid_t pid = getpid(); // Получаем идентификатор процесса
    pthread_t tid = pthread_self(); // Получаем идентификатор потока

    for (int i = 0; i < 75; i++) {
        printf("Thread OS04_11_T1 - PID: %d, TID: %lu, Iteration: %d\n", pid, tid, i + 1);
        sleep(1); // Задержка в 1 сек
    }

    return NULL; // Завершение потока
}

int main() {
    pid_t pid = getpid(); // Получаем идентификатор процесса
    pthread_t thread;     // Идентификатор потока

    // Создаем поток, который будет выполнять функцию OS04_10_T1
    if (pthread_create(&thread, NULL, OS04_11_T1, NULL) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    // Основной процесс выполняет цикл 100 итераций
    for (int i = 0; i < 100; i++) {
        printf("Main process - PID: %d, Iteration: %d\n", pid, i + 1);
        sleep(1); // Задержка в 1 сек
    }

    // Ожидаем завершения потока
    if (pthread_join(thread, NULL) != 0) {
        perror("Failed to join thread");
        return 1;
    }

    return 0;
}
