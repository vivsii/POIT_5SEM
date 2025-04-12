#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <sys/inotify.h>
#include <errno.h>

#define DEFAULT_LOG_DIR "/var/log/service09d"
#define CONFIG_FILE "/etc/service09d/config.conf"
#define EVENT_BUF_LEN (1024 * (sizeof(struct inotify_event) + 16))

// Глобальные переменные
char log_dir[256] = DEFAULT_LOG_DIR;
char watch_dir[256] = "";
int inotify_fd = -1;
int watch_descriptor = -1;

// Функция для записи в лог
void write_log(const char *log_file, const char *message, int include_errno) {
    FILE *log = fopen(log_file, "a");
    if (!log) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    fprintf(log, "[%02d:%02d:%02d] %s",
            local_time->tm_hour, local_time->tm_min, local_time->tm_sec, message);
    if (include_errno) {
        fprintf(log, ": %s (errno=%d)", strerror(errno), errno);
    }
    fprintf(log, "\n");
    fclose(log);
}

// Функция для загрузки конфигурации
void load_config() {
    FILE *config = fopen(CONFIG_FILE, "r");
    if (!config) {
        write_log("/var/log/service09d/srv.log", "Failed to open configuration file", 1);
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), config)) {
        if (strncmp(line, "log_dir=", 8) == 0) {
            strncpy(log_dir, line + 8, sizeof(log_dir));
            log_dir[strcspn(log_dir, "\n")] = '\0'; // Убираем символ новой строки
        } else if (strncmp(line, "watch_dir=", 10) == 0) {
            strncpy(watch_dir, line + 10, sizeof(watch_dir));
            watch_dir[strcspn(watch_dir, "\n")] = '\0';
        }
    }

    fclose(config);
}

// Обработчик сигнала SIGTERM
void handle_sigterm(int sig) {
    (void)sig; // Для подавления предупреждения
    write_log("/var/log/service09d/srv.log", "Демон завершил свою работу.", 0);
    if (inotify_fd >= 0) close(inotify_fd);
    exit(EXIT_SUCCESS);
}

// Обработчик сигнала SIGHUP
void handle_sighup(int sig) {
    (void)sig;
    write_log("/var/log/service09d/srv.log", "Демон перезагружает конфигурацию.", 0);
    load_config();
}

// Основной цикл работы демона
void run_daemon() {
    char srv_log[512], dir_log[512];
    snprintf(srv_log, sizeof(srv_log), "%s/srv.log", log_dir);
    snprintf(dir_log, sizeof(dir_log), "%s/dir.log", log_dir);

    if (mkdir(log_dir, 0755) == 0) {
        write_log(srv_log, "Успех! Демон создал каталог для логов.", 0);
    } else if (errno != EEXIST) {
        write_log(srv_log, "Попытка создания каталога логов провалилась!", 1);
        exit(EXIT_FAILURE);
    } else {
        write_log(srv_log, "Успех! Каталог логов уже существует.", 0);
    }
    write_log(srv_log, "Путь к каталогу для отслеживания: %s", watch_dir);

    // Проверка и создание каталога для отслеживания
    write_log(srv_log, "Debug: Checking if watch directory exists... (%s)", watch_dir);
    if (access(watch_dir, F_OK) != 0) {
        write_log(srv_log, "Каталог для отслеживания не существует. Попытка создать...", 0);
        if (mkdir(watch_dir, 0755) == 0) {
            write_log(srv_log, "Каталог для отслеживания успешно создан.", 0);
        } else {
            write_log(srv_log, "Не удалось создать каталог для отслеживания!", 1);
            exit(EXIT_FAILURE);
        }
    }

    inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        write_log(srv_log, "Failed to initialize inotify", 1);
        exit(EXIT_FAILURE);
    }

    watch_descriptor = inotify_add_watch(inotify_fd, watch_dir, IN_CREATE | IN_DELETE | IN_MODIFY);
    if (watch_descriptor < 0) {
        write_log(srv_log, "Ошибка отслеживания каталога!", 1);
        close(inotify_fd);
        exit(EXIT_FAILURE);
    }

    write_log(srv_log, "Успех! Демон запущен.", 0);
    char buffer[EVENT_BUF_LEN];
    while (1) {
        int length = read(inotify_fd, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            write_log(srv_log, "Failed to read inotify events", 1);
            break;
        }

        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len) {
                char message[512];
                if (event->mask & IN_CREATE) {
                    snprintf(message, sizeof(message), "Создан файл: %s", event->name);
                } else if (event->mask & IN_DELETE) {
                    snprintf(message, sizeof(message), "Удалён файл: %s", event->name);
                } else if (event->mask & IN_MODIFY) {
                    snprintf(message, sizeof(message), "Изменён файл: %s", event->name);
                }
                write_log(dir_log, message, 0);
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }
}

// Демонизация процесса
void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    if (setsid() < 0) exit(EXIT_FAILURE);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");
    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) close(x);
}

int main() {
    signal(SIGTERM, handle_sigterm);
    signal(SIGHUP, handle_sighup);

    daemonize();
    load_config();
    run_daemon();
    return 0;
}
