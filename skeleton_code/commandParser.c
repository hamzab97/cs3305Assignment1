/*
name: Hamza Bana
email: hbana3@uwo.ca

commandParser.c contains all the methods for parsing the user input
*/

#include "commandParser.h"

//parse spaces " " in user input to obtain the commands only
void parseSpaces(char *str, char **resultArr, int *i){
  // printf("\nspaces method called\n");
  //user char *strsep(char **stringp, const char *delim) from string.h library
  // printf("before loop result array at index %d is %s and str is %s\n", *i, resultArr[*i], str);
  char *token;
  token = strsep(&str, " ");
  while (token){
    resultArr[(*i)++] = token;
    // printf("i: %d, token is %s\n", *i, token);
    token = strsep(&str, " ");
  }
  resultArr[(*i)++] = "/0"; //mark with a 0 character to mark end of array
  // for (int j = 0; j < *i; j ++)
  //   printf("result arr %d: %s\n", j, resultArr[j]);
  // printf("spaces method end\n");
}

//parse and look for pipe filters "|" in user input
//if true is returned, means that there was a pipe filter
//if pipe filter then pipearray index 0 will be the command before the pipe
//and index 1 will be the command after the pipe
//if no pipe filter then return false is returned
void parsePipeFilter(char *line, char **pipeArray, char **pipeArrayCopy){
  // printf("\npipe filter method called\n");
  char *token;
  int j = 0;
  while ((token = strsep(&line, "|")) != NULL){
    // printf("j: %d, token: %s\n", j, token);
    pipeArray[j] = token;
    pipeArrayCopy[j] = token;
    j++;
  }
  pipeArray[j] = "/0";
  pipeArrayCopy[j] = "/0";
  // printf("pipe filter method end\n");
}

//parse and look for ">" io redirection in user input
void parseIORedirection(char *line){

}

void concatenate(char *original, char *add){
  while (*original)
    original++;
  while (*add){
    *original = *add;
    add++;
    original++;
  }
  *original = "\0";
}
