#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/commands.h"

struct listCommands parseCommands(char *commandLine) {
    struct listCommands list;
    list.cnt = 0;
    list.commands = malloc(sizeof(struct singleCommand));

    char* current = commandLine;
    char* singleCommand;

    char *spilit;
    while((spilit = strpbrk(current, "|&")) != NULL) {
        list.commands = realloc(list.commands, (list.cnt + 1) * sizeof(struct singleCommand));
        
        int len = spilit - current;

        singleCommand = malloc(len + 1);

        strncpy(singleCommand, current, len);

        singleCommand[len - 1] = '\0';

        list.commands[list.cnt] = parseSingleCommand(singleCommand);

        if(*spilit == '|') {
            list.commands[list.cnt].pipeFlag = 1;
        } else {
            list.commands[list.cnt].ampFlag = 1;
        }
        list.cnt++;
        current = spilit + 1;
        free(singleCommand);
    }
    if(spilit == NULL) {
        struct singleCommand commands = parseSingleCommand(current);
        if(commands.parsedCmd[0] != NULL) {
            list.commands = realloc(list.commands, (list.cnt + 1) * sizeof(struct singleCommand));
            list.commands[list.cnt] = commands;
            list.cnt++;
        }
    }

    return list;
}

struct singleCommand parseSingleCommand(char *commandLine) {
    struct singleCommand commands;
    char* savePtr;

    commands.inputFlag = 0;
    commands.outputFlag = 0;
    commands.appendFlag = 0;
    commands.pipeFlag = 0;
    commands.ampFlag = 0;
    commands.errorFlag = 0;
    commands.inputFile = NULL;
    commands.outputFile = NULL;

    char *current = strtok_r(commandLine, " ", &savePtr);

    if(current != NULL) {
        current = strdup(current);
        commands.parsedCmd = malloc(sizeof(char*));
        commands.cnt = 1;
        commands.parsedCmd[0] = current;
    } else {
        commands.parsedCmd = NULL;
        commands.cnt = 0;
    }

    while(current != NULL) {
        current = strtok_r(NULL, " ", &savePtr);
        if(current != NULL) {
            current = strdup(current);
            
            // "> case"
            if(strcmp(current, ">") == 0) {
                commands.outputFlag = 1;
                current = strtok_r(NULL, " ", &savePtr);
                if(current != NULL) {
                    commands.outputFile = strdup(current);
                } else {
                    commands.errorFlag = 1;
                    break;
                }
            }
            // "< case"
            else if(strcmp(current, "<") == 0) {
                commands.inputFlag = 1;
                current = strtok_r(NULL, " ", &savePtr);
                if(current != NULL) {
                    commands.inputFile = strdup(current);
                } else {
                    commands.errorFlag = 1;
                    break;
                }
            }
            // ">> case"
            else if(strcmp(current, ">>") == 0) {
                commands.appendFlag = 1;
                current = strtok_r(NULL, " ", &savePtr);
                if(current != NULL) {
                    commands.outputFile = strdup(current);
                } else {
                    commands.errorFlag = 1;
                    break;
                }
            }
            else {
                commands.cnt++;
                commands.parsedCmd = realloc(commands.parsedCmd, commands.cnt * sizeof(char*));
                commands.parsedCmd[commands.cnt - 1] = current;
            }
        }
    }
    commands.parsedCmd = realloc(commands.parsedCmd, (commands.cnt + 1) * sizeof(char*));
    commands.parsedCmd[commands.cnt] = NULL;
    return commands;
}
