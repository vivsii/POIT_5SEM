#include <iostream>
#include <thread>
#include <sched.h>
#include <unistd.h>
#include <vector>

int main() {
    std::cout << "Evseenko Viktoria" << std::endl;
    // Получение идентификатора текущего процесса
    pid_t process_id = getpid();
    std::cout << "Current process ID: " << process_id << std::endl;

    // Получение идентификатора текущего потока
    std::thread::id thread_id = std::this_thread::get_id();
    std::cout << "Current (main) thread ID: " << thread_id << std::endl;

    // Получение приоритета (nice) текущего потока
    int nice_value = nice(0); // 0 — это текущий приоритет
    std::cout << "Priority (nice) of the current thread: " << nice_value << std::endl;

    // Получение номера доступных процессоров
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    if (sched_getaffinity(0, sizeof(cpu_set), &cpu_set) == 0) {
        std::cout << "Available CPU numbers: ";
        for (int i = 0; i < CPU_SETSIZE; ++i) {
            if (CPU_ISSET(i, &cpu_set)) {
                std::cout << i << " ";
            }
        }
        std::cout << std::endl;
    }
    else {
        std::cerr << "Error getting processor information" << std::endl;
    }

    return 0;
}
