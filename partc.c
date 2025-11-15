#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: partc FILEIN\n");
        return 1;
    }

    int pipefd[2];

    if (pipe(pipefd) < 0) {
        perror("pipe");
        return 1;
    }

    int pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(pipefd[1]);

        if (dup2(pipefd[0], STDIN_FILENO) < 0) {
            perror("dup2 child");
            close(pipefd[0]);
            return 1;
        }

        close(pipefd[0]);

        char* sortArgs[] = { "sort", "-t,", "-k2", "-n", NULL };
        execvp("sort", sortArgs);

        perror("execvp sort");
        return 1;
    }
    
    close(pipefd[0]);

    if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
        perror("dup2 parent");
        close(pipefd[1]);
        return 1;
    }

    close(pipefd[1]);

    char* partaArgs[] = { "./parta", argv[1], NULL };
    execv("./parta", partaArgs);

    perror("execv ./parta");

    waitpid(pid, NULL, 0);
    return 1;
}
