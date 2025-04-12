//Lab-06f
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <errno.h>

#define BUFFER_SIZE (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int IsDirectory(const char* path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) {
        perror("Error while reading information about file");
        return 0;
    }
    return S_ISDIR(statbuf.st_mode);
}

void ListDirectoryContents(const char* dirPath) {
    DIR* dir = opendir(dirPath);
    struct dirent* entry;

    if (dir == NULL) {
        perror("Error while open catalog");
        return;
    }

    printf("Catalog content '%s':\n", dirPath);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s\n", entry->d_name);
            char fullPath[PATH_MAX];
            snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);
            if (IsDirectory(fullPath)) {
                ListDirectoryContents(fullPath);
            }
        }
    }
    closedir(dir);
}

void MonitorDirectory(const char* dirPath) {
    int fd = inotify_init();
    if (fd == -1) {
        perror("Could not initialize inotify");
        return;
    }

    int wd = inotify_add_watch(fd, dirPath, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM);
    if (wd == -1) {
        perror("Could not add watch");
        close(fd);
        return;
    }

    char buffer[BUFFER_SIZE];
    printf("Start monitoring changes in catalog '%s'. Press Ctrl+C for leave.\n", dirPath);

    while (1) {
        int length = read(fd, buffer, sizeof(buffer));
        if (length < 0) {
            perror("Error while reading event");
            break;
        }

        for (int i = 0; i < length;) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    printf("File or Catalog added: %s\n", event->name);
                }
                else if (event->mask & IN_DELETE) {
                    printf("File or catalog removed: %s\n", event->name);
                }
                else if (event->mask & IN_MODIFY) {
                    printf("File modified: %s\n", event->name);
                }
                else if (event->mask & IN_MOVED_TO) {
                    printf("File moved to catalog: %s\n", event->name);
                }
                else if (event->mask & IN_MOVED_FROM) {
                    printf("File moved from catalog: %s\n", event->name);
                }
                else {
                    printf("Other events: %u, file: %s\n", event->mask, event->name);
                }
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Using: %s <path to catalog>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* dirPath = argv[1];

    ListDirectoryContents(dirPath);
    MonitorDirectory(dirPath);

    return EXIT_SUCCESS;
}