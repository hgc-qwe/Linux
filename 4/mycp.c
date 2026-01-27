#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int inputFd, outputFd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        perror("open source");
        exit(EXIT_FAILURE);
    }

    outputFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (outputFd == -1) {
        perror("open destination");
        close(inputFd);
        exit(EXIT_FAILURE);
    }


    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        ssize_t numWrite;
        char* ptr = buf;
        size_t remaining = numRead;

        while (remaining > 0) {
            numWrite = write(outputFd, ptr, remaining);
            if (numWrite == -1) {
                perror("write");
                close(inputFd);
                close(outputFd);
                exit(EXIT_FAILURE);
            }
            remaining -= numWrite;
            ptr += numWrite;
        }
    }

    if (numRead == -1) {
        perror("read");
        close(inputFd);
        close(outputFd);
        exit(EXIT_FAILURE);
    }

    if (close(inputFd) == -1) perror("close input");
    if (Close(outputFd) == -1) perror("close output");

    return 0;
}