#include "mush.h"
#define MAX_CMD_LINE 514

int main(int argc, char *argv[]) {
  /*  char c;
    int i = 0;
    int fd;*/
    char *buffer;
    char *scriptBuff = NULL;
    FILE *file;
    char buff[514]; /**/

    int size;
    if (argc > 2) {
        fprintf(stderr, "usage: %s [ scriptfile ]\n", argv[0]);
        exit(1);
    }
    if (argc == 1) {
        if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO)) {            
            printf("8-P ");
           /* scanf(" %s", sdinBuff);*/
            if((buffer = (char*)malloc(513)) == NULL){
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            file = stdin;
            while((buffer = readcmdline(file)) !=  NULL){ 
             /*   if(feof(sdinBuff)){
                    * we reached the end of file */ /* MGHT NOT WANT *
                    printf('\n');
                    return 0; * we are successfully done *
                } */
               /* if(strlen(buffer) != 0){
                    *realloc as well*
                    *pipe here*
                }*/
              /*  fprintf(stdout, "%s\n", buffer);*/
               
                if(buffer){ /*if initialized, free */                      
                    free(buffer);
                }
             
                
                printf("8-P ");
            /*    fflush(stdout); */
            }                
        }

        printf("\n");
        return 0;
    }
    
    
    else {
        /* read script file line by line
         * toss each line into parseline */ 
       /* if(fd = open(argv[1],  O_RDONLY, S_IRUSR | S_IRGRP | S_IROTH) == -1){
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }

        size = lseek(fd, 0, SEEK_END) + 1; 
        if(size == 0 || (lseek(fd, 0, SEEK_SET) == -1)){
            perror("lseek");
            exit(EXIT_FAILURE);
        }
        if((scriptBuff = (char *)malloc(size)) == NULL){
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        memset(scriptBuff, 0, sizeof(scriptBuff)+1);
        if(read(fd, scriptBuff, size) == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }*/
        file = fopen(argv[1], "r");
        while (fgets(buff, MAX_CMD_LINE, file)) {
            readcmdline(buff);
            memset(buff, 0, MAX_CMD_LINE);
        }
    }
    return 0;
}


