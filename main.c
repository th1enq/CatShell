/*
    - Author: th1enq
    - Date: 2024-20-01
    - Project: Cat-Shell
    - Operating System
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "../include/commands.h"
#include "../include/execute.h"

// Running = 1
// Not Running = 0
int running = 1;

char *command = NULL;

size_t len = 0;

int main(void) {
    while(running) {
        
        fprintf(stdout, "Cat_Shell$ >");
        fflush(stdout);
        
        ssize_t size = getline(&command, &len, stdin);

        if(size > 1) {
            if(command[size - 1] == '\n') {
                command[size - 1] = '\0';
            }

            struct listCommands commands = parseCommands(command);

            int pipefd[2] = {-1, -1};

            for(int i = 0; i < commands.cnt; i++) {
                FILE *write = stdout;
                FILE *read = stdin;

                if(pipefd[0] != -1) {
                    read = fdopen(pipefd[0], "rb");
                }

                if(commands.commands[i].pipeFlag) {
                    pipe(pipefd);
                    write = fdopen(pipefd[1], "wb");
                } else {
                    pipefd[0] = pipefd[1] = -1;
                }

                if(commands.commands[i].errorFlag) {
                    fprintf(stderr, "An error has occurred\n");
                    continue;
                }

                if(commands.commands[i].outputFlag) {
                    write = fopen(commands.commands[i].outputFile, "wb");
                    if(write == NULL) {
                        fprintf(stderr, "An error has occurred\n");
                    }
                }

                if(commands.commands[i].inputFlag) {
                    read = fopen(commands.commands[i].inputFile, "rb");
                    if(read == NULL) {
                        fprintf(stderr, "An error has occurred\n");
                    }
                }

                if(commands.commands[i].appendFlag) {
                    write = fopen(commands.commands[i].outputFile, "ab");
                    if(write == NULL) {
                        fprintf(stderr, "An error has occurred\n");
                    }
                }

                if(strcmp(commands.commands[i].parsedCmd[0], "exit") == 0) {
                    running = 0;
                } else {
                    executeCommand(commands.commands[i], write, read);
                }
                free(commands.commands[i].outputFile);
                free(commands.commands[i].inputFile);
                for(int j = 0; j < commands.commands[i].cnt; j++) {
                    free(commands.commands[i].parsedCmd[j]);
                }
                free(commands.commands[i].parsedCmd);
                if(write != stdout) {
                    fclose(write);
                }
                if(read != stdin) {
                    fclose(read);
                }
            }
            free(commands.commands);
        }
    }
}
