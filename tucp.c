#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>

// takes a source file and copies it to the destination file (2 arguments)
void copyFile(const char* sourceFile, const char* destinationFile) {
    int sourceFileDirectory, destFileDirectory;
    int dataRead;
    char buffer[BUFSIZ];

    // open the source file in read only permissions 
    sourceFileDirectory = open(sourceFile, O_RDONLY);
    // if unable to open source file, print error and exit
    if (sourceFileDirectory == -1) {
        perror("Error : opening source file");
        exit(1);
    }

    // open the destination file for writing only, create if doesn't exist, 
    // truncate to zero length if it exists, and give reading permission
    destFileDirectory = open(destinationFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR);
    // if unable to open destination file, print error and exit
    if (destFileDirectory == -1) {
        perror("Error : opening destination file");
        exit(1);
    }

    // while there is data read and write into the buffer and into the destination file
    while ((dataRead = read(sourceFileDirectory, buffer, BUFSIZ)) > 0) {
        if (write(destFileDirectory, buffer, dataRead) != dataRead) {
            perror("Error : writing to destination file");
            exit(1);
        }
    }

    // if error reading source file, print error and exit
    if (dataRead == -1) {
        perror("Error : reading from source file");
        exit(1);
    }

    // if error closing source file, print error and exit
    if (close(sourceFileDirectory) == -1) {
        perror("Error : closing source file");
        exit(1);
    }

    // if error closing destination file, print error and exit
    if (close(destFileDirectory) == -1) {
        perror("Error : closing destination file");
        exit(1);
    }
}

void tucp(int argc, char* argv[]) {
    // if argument is less than three than missing arguments (includes ./tucp), print error and exit
    if (argc < 3) {
        printf("Usage: ./tucp source destination\n");
        exit(1);
    }

    // open destination directory 
    DIR *dir = opendir(argv[argc - 1]);
    
    if (dir != NULL) {
        // close directory
        closedir(dir);
        for (int i = 1; i < argc - 1; i++) {
            const char* source = argv[i];
            const char* filename = strrchr(source, '/');
            // create buffer for destination
            char destination[PATH_MAX];

            if (filename != NULL) {
                strcpy(destination, argv[argc - 1]);
                strcat(destination, filename);
            } else {
                strcpy(destination, argv[argc - 1]);
                strcat(destination, "/");
                strcat(destination, source);
            }

            // copy source file to destination path
            copyFile(source, destination);
        }
    } 
    
    else {
        // if the destination is a file, copy the contents of the first source file to the destination file
        copyFile(argv[1], argv[2]);
    }
}

int main(int argc, char* argv[]) {
    tucp(argc, argv);
    return 0;
}
