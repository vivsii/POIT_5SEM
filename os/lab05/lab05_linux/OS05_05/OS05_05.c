#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    // Длительный цикл
    while (1) {
        // Просто задержка, чтобы уменьшить нагрузку на процессор
        printf("Working...\n");
        fflush(stdout); // Сброс буфера, чтобы вывод сразу отображался
        sleep(1); // Задержка 1 секунда
    }

    return 0;
}
