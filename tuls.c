#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

// function to list files in a directory
void listFiles(const char *path, int indent) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    // if directory null then exit
    if (dir == NULL) {
        printf("tuls: cannot open directory\n");
        exit(1);
    }

    // iterates through each entry in a directory until entry null
    while ((entry = readdir(dir)) != NULL) {
        // ignore current directory (.) and parent directory (..) to avoid infinite recursion
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // indent the output based on the level of recursion
            for (int i = 0; i < indent; i++) {
                printf(" ");
            }
            // print the name of the file or directory
            printf("->%s\n", entry->d_name);

            // create the new path by concatenating the current path and the entry name
            // stores result in newPath array
            char pathArray[1000];
            snprintf(pathArray, sizeof(pathArray), "%s/%s", path, entry->d_name);

            struct stat statBuf;
            // uses stat() to get information about file or directory
            if (stat(pathArray, &statBuf) == 0 && S_ISDIR(statBuf.st_mode)) {
                // if the entry is a directory then call listFiles
                listFiles(pathArray, indent + 2);
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
    }
    return 0;
}