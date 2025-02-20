#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <dirent.h> 
#include <errno.h> 
#include "../include/execute.h"


void executeCommand(struct singleCommand command, FILE *write, FILE *read) {
    int pid = fork();
    int fileNumWrite = fileno(write);
    int fileNumRead = fileno(read);

    if(pid < 0) {
        fprintf(stderr, "An error has occurred\n");
    } else if(pid == 0) {
        if(write != stdout) {
            dup2(fileNumWrite, STDOUT_FILENO);
        }
        if(read != stdin) {
            dup2(fileNumRead, STDIN_FILENO);
        }

        execvp(command.parsedCmd[0], command.parsedCmd);
        fprintf(stderr, "An error has occurred\n");
        exit(1);
    } else {
        if(command.ampFlag == 0) {
            waitpid(pid, NULL, 0);
        }
    }
}