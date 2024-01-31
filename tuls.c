#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

// function to list files in a directory
void listFiles(const char *path, int indent) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    // if directory error then exit
    if (dir == NULL) {
        printf("tuls: cannot open directory\n");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        // ignore current directory (.) and parent directory (..)
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // indent the output based on the level of recursion
            for (int i = 0; i < indent; i++) {
                printf(" ");
            }
            // print the name of the file or directory
            printf("->%s\n", entry->d_name);

            // create the new path by concatenating the current path and the entry name
            char newPath[1024];
            snprintf(newPath, sizeof(newPath), "%s/%s", path, entry->d_name);

            struct stat statBuf;
            // get information about the file or directory using stat()
            if (stat(newPath, &statBuf) == 0 && S_ISDIR(statBuf.st_mode)) {
                // if the entry is a directory, recursively call listFiles() on it
                listFiles(newPath, indent + 2);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // if no command line argument is provided, list files in the current directory
        listFiles(".", 0);
    } else if (argc == 2) {
        // if a directory path is provided as a command line argument, list files in that directory
        listFiles(argv[1], 0);
    } else {
        // display the correct usage of the program if the number of arguments is incorrect
        printf("Usage: ./tuls [directory]\n");
        exit(1);
    }

    return 0;
}