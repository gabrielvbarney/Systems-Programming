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




int countStages(char *cmdline){
    char* token;
    int stages = 0;

    if((token = strtok(cmdline, "|")) == NULL){
        return stages;
    }
    stages++;
    while((token = strtok(NULL, "|")) != 0){
        stages++;
    }

    return stages;
}


int main(){
    char s[] = "a a  a a | d | ddddd dddd | ddd fdsaf | sdas";
    char t[] = "a a a";
    char r[] = "a | B";
    printf("Total Stages: %d\n", countStages(s));
    printf("Total Stages: %d\n", countStages(t));
    printf("Total Stages: %d\n", countStages(r));
}


