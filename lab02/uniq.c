/*  Name: Gabriel Barney  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *uniq(FILE *file, char *prevLine) {
    
    int size = 256;
    int charNum = 0;
    char *lineBuff = (char*)malloc(sizeof(char)*size);
    char c;

    c = fgetc(file);
    
    if (c == EOF) {
        free(prevLine);
        fclose(file);  
        exit(0);
    }
    
    while ((c != '\n')) {
        
        if (charNum == size) {
            size += 256;
            lineBuff = realloc(lineBuff, size);
        }
        
        lineBuff[charNum] = c;
        charNum++;
        c = fgetc(file);
    
    }     

    lineBuff[charNum] = '\0';
   
    if (strlen(lineBuff) < strlen(prevLine)) {
        printf("%s\n", lineBuff);
    }
    else if (strncmp(lineBuff, prevLine, charNum) != 0) {
        printf("%s\n", lineBuff);
    }
  
    prevLine = (char*)realloc(prevLine, strlen(lineBuff) + 1); 
    strncpy(prevLine, lineBuff, charNum);
    free(lineBuff);
    
    return prevLine;
}
 

int main(int argc, char* argv[]) {
    
    FILE *file;
    int size = 256;
    char *prevLine = (char*)malloc(sizeof(char)*size);
    
    file = fopen(argv[1], "r");
    
    if (!file) {
        printf("Error: File not found.\n");
        exit(1);
    }                       
    
    while (1) {
        prevLine = uniq(file, prevLine);   
    } 
    
/*    free(prevLine);
    fclose(file);  */
    
    return 0;   
} /**/
