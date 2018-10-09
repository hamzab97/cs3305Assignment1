/*
name: Hamza Bana
email: hbana3@uwo.ca

commandParser.h contains all the methods for parsing the user input
*/

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void parseSpaces(char *line, char **resultArr, int *i);
void parsePipeFilter(char *line, char **pipeArray, char **pipeArrayCopy);
void parseIORedirection(char *line);
void concatenate(char *original, char *add);

#endif
