#ifndef COMMANDS_H
#define COMMANDS_H

struct singleCommand {
    int cnt;
    
    char *inputFile;
    char *outputFile;

    int inputFlag;
    int outputFlag;
    int appendFlag;
    int pipeFlag;
    int ampFlag;
    int errorFlag;

    char **parsedCmd;
};

struct listCommands {
    int cnt;
    struct singleCommand *commands;
};

struct listCommands parseCommands(char *commandLine);
struct singleCommand parseSingleCommand(char *commandLine);

#endif