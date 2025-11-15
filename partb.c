#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "USAGE: partb FILEIN FILEOUT\n");
        return 1;
    }

    char* inputFile  = argv[1];
    char* outputFile = argv[2];

    int fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0770);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(fd);
        return 1;
    }

    close(fd);

    printf("Category,Count\n");
    fflush(stdout);

    char* args[] = { "./parta", inputFile, NULL };

    execv("./parta", args);

    perror("execv");
    return 1;
}
