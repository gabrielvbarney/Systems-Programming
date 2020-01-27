#include "tryit.h"

int main(int argc, char *argv[]) {
    int forkVal;
    int execVal;
    
    if (argc == 1 || argc > 2) {
        fprintf(stderr, "usage: tryit command\n");
        exit(1);
    }
    if (access(argv[1], F_OK)) {
        printf("%s: No such file or directory\n", argv[1]);
        fprintf(stderr, "Process %d exited with an error value.\n", getpid());
        exit(EXIT_FAILURE);
    }

    execVal = 0;
    if (!(forkVal = fork())) {    
        execVal = execl(argv[1], argv[1], NULL);
    } else {
        wait(&execVal);
        if (WEXITSTATUS(execVal) != 0) {
            fprintf(stderr, 
                    "Process %d exited with an error value.\n", getpid());
            exit(EXIT_FAILURE);
        } else {
            printf("Process %d succeeded.\n", getpid());
        }
    } 
    return 0;
}   
