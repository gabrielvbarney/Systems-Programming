#include "mush.h"

char *readcmdline(FILE *file){
    char *buffer;
   /* char *cmdline;*/
    char c = '\0';
    int i = 0;

    if((buffer = (char*)malloc(sizeof(char) * 512)) == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    memset(buffer, '\0', 512);
    fopen(file, "r");
    while(((c = fgetc(file)) != EOF) && c != '\n'){

        buffer[i] = c;
        i++;
        printf("%d\n", c);
        
        if ((int)c == 3) {
            return NULL;
        }
    }
    buffer[i+1] = '\0';

    if(!feof(file) && c != '\n'){ /*exited but not EOF*/
        free(buffer); /* exited due to error / crtl-d */
        return NULL; 
    }
    
    if(strlen(buffer) == 0){
        if(c == '\n'){
            buffer[0] = '\n';
            buffer[1] = '\0';
            return buffer; 
        }
        free(buffer); /* exited due to error / crtl-d */
        return NULL; 

    }
      
    else{ /*exited due to new line or EOF */
        buffer = (char*)realloc(buffer, (strlen(buffer) + 1) * sizeof(char));
        if(buffer[strlen(buffer)-1] == '\n'){
            buffer[strlen(buffer)-1] = '\0';
        }


      /*  cmdline = strdup(buffer); */
        return buffer;         
    }

}



