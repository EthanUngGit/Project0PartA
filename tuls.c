#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

// Function to list files in a directory
void listFiles(const char *path, int indent) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        printf("tuls: cannot open directory\n");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            for (int i = 0; i < indent; i++) {
                printf(" ");
            }
            printf("->%s\n", entry->d_name);

            char newPath[1024];
            snprintf(newPath, sizeof(newPath), "%s/%s", path, entry->d_name);

            struct stat statBuf;
            if (stat(newPath, &statBuf) == 0 && S_ISDIR(statBuf.st_mode)) {
                listFiles(newPath, indent + 2);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        listFiles(".", 0);
    } else if (argc == 2) {
        listFiles(argv[1], 0);
    } else {
        printf("Usage: ./tuls [directory]\n");
        exit(1);
    }

    return 0;
}