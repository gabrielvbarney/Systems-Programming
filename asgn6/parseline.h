#ifndef PARSELINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <signal.h>

#define MAX_CMD_LINE 512
#define MAX_CMDS 10
#define MAX_ARGS 10
#define IN_ASCII 60
#define OUT_ASCII 62
#define PIPE_ASCII 124
#define SPACE_ASCII 32
/*#define PIPE_OUTPUT "pipe to stage \0"
#define PIPE_INPUT  "pipe from stage \0"
#define STDIN "original stdin\0" 
#define STDOUT "original stdout\0"*/

typedef struct {
    char *cmdLine;                  /* command line */
    char stageName[512]; 
    char *cmds[MAX_CMDS + 1];        /* broken-up commands */ 
    char *_argv[MAX_ARGS + 1];      /* arguments */ 
    int _argc;                      /* number of arguements */
    int input;   /* input string */
    int output;  /* output string */
} Parser;

/*typedef struct{
    int numStages; 
    Parser *pipe[11];
} Pipeline;*/ /* Didn't implement but might want to later */

void processPipe(char* cmdline);

void processStage(char* token, int num, int total);

void getCmds(char* token, Parser *parser);

void getInput(char* token, Parser *parser);

void getOutput(char* token, Parser *parser);

void processSingle(char *cmdline);

void int_to_ascii(int stageNum, char* str);

int errorProcess(char* cmd_line);

void printStage(Parser *parser, int num);

void getArgs(Parser *parser, char* stage);

void clearParser(Parser *parser);





#endif
