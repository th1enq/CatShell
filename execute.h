#ifndef EXECUTE_H
#define EXECUTE_H

#include "commands.h"
#include <stdio.h>

void executeCommand(struct singleCommand command, FILE* write, FILE* read);

#endif