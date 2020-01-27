#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h> /* for isalpha */

#include "hash_set.h"

/*
char *read_word_file(FILE *file) {
    int size = 128;  rileys comment from lab  2 
    char *word;
    char *wordBuff = (char*)malloc(size * sizeof(char));  check for error NULL 
    int wordLen = 0;
    int ch = getc(file);

    if (wordBuff == NULL) {
        perror("Cannot allocate word buffer.");
        exit(-1);
    } 
    while ((ch != EOF) && (isalpha(ch))) {
        ch = tolower(ch);
        if (wordLen >= size-1) {
            size *= 2;
            wordBuff = (char*)realloc(wordBuff, size * sizeof(char));
            if (wordBuff == NULL) {
                perror("Cannot reallocate word buffer.");
                exit(-1);
            }
        }
        if(ch == EOF){
            free(wordBuff);
            return NULL; 
        }
        wordBuff[wordLen] = ch;
        wordLen++;
        ch = getc(file);
    }
    
    wordBuff[wordLen] = '\0';
    word = wordBuff;
    free(wordBuff);
    
    if (wordLen == 0) {
        free(word);
        return NULL;
    } 
    else {
        return word;
    }
}
*/
int read_word_file(FILE *file, char **wordBuff) {
    int size = 128; /* rileys comment from lab 2 */
    int wordLen = 0;
    int ch;

    *wordBuff  = (char*)malloc(size * sizeof(char));

    if (*wordBuff == NULL) {
        perror("Cannot allocate word buffer.");
        exit(-1);
    }

    /* write while loop for getchar() in main */
    while((ch = fgetc(file)) != EOF && isalpha(ch)){
        if(!islower(ch)){  /* We understand that there is a tolower, but
                            * this was sexier looking. Sometimes it
                            * costs more to look good */
            ch += 32; 
        }
        if (wordLen >= size-1) {
            size *= 2;
            *wordBuff = (char*)realloc(*wordBuff, size * sizeof(char));
            if (*wordBuff == NULL) {
                perror("Cannot reallocate word buffer.");
                exit(-1);
            }
        }
        (*wordBuff)[wordLen] = ch;
        wordLen++;
    }
        
    (*wordBuff)[wordLen] = '\0';

    if(!wordLen) {
        free(*wordBuff);
        *wordBuff = NULL;
    }

    if(ch == EOF){
        return 0;
    }
    
    return 1;
}
int read_word_kb(char **wordBuff) {
    int size = 128; /* rileys comment from lab 2 */
    int wordLen = 0;
    int ch;

    *wordBuff = (char*)malloc(size * sizeof(char));

    if (*wordBuff == NULL) {
        perror("Cannot allocate word buffer.");
        exit(-1);
    }

    /* write while loop for getchar() in main */
    while((ch = getchar()) != EOF && isalpha(ch)){
        if(!islower(ch)){  /* We understand that there is a tolower, but
                            * this was sexier looking. Sometimes it
                            * costs more to look good */
            ch += 32; 
        }
        if (wordLen >= size-1) {
            size *= 2;
            *wordBuff = (char*)realloc(*wordBuff, size * sizeof(char));
            if (*wordBuff == NULL) {
                perror("Cannot reallocate word buffer.");
                exit(-1);
            }
        }
        (*wordBuff)[wordLen] = ch;
        wordLen++;
    }
        
    wordBuff[wordLen] = '\0';
    word = wordBuff;
/*    free(wordBuff);*/
    
    if (wordLen == 0) {
        return NULL;
    } else {
        return word;
    (*wordBuff)[wordLen] = '\0';

    if(!wordLen) {
        free(*wordBuff);
        *wordBuff = NULL;
    }

    if(ch == EOF){
        return 0;
    }
    
    return 1;
}
            
int main(int argc, const char* argv[]) {
    int numTopWords;
    int cmd_i; /* for looping thru files */
    int i; /* for loop itself */
    FILE *file;
    
    char *word; 
    
     if (argc == 1) { 
           while(read_word_kb(&word)){
                if(word != NULL){
                    insert(word);
                }
           }
           if (word != NULL){
               insert(word);
           }
           free(word);
           print_hash_table();
    }
     else if (argc >= 2) {
        if (argv[1][0] == '-') {
            if (argv[1][1] != 'n' || isalpha(argv[2][0])) {
                fprintf(stderr, "usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
                exit(EXIT_FAILURE);
            }
            else {
                numTopWords = sscanf(argv[2], "%d\n", &numTopWords);
                cmd_i = 3;
                printf("num of top words: %d\n", numTopWords);
            }
        } 
        else {
            numTopWords = 10;
            cmd_i = 1;
        }
        for (i = cmd_i; i < argc; i++) {
            file = fopen(argv[i], "r");
            if (file == NULL) {
                fprintf(stderr, "%s: No such file or directory\n", argv[i]);
            } 
            else {
               /* while((word = read_word_file(file)) != NULL){
                    insert(word);*/
                while(read_word_file(file, &word)){
                        if(word != NULL){
                            insert(word);
                        }
                }
                if(word != NULL){
                    insert(word);
                }
                free(word);
            }   
            fclose(file);
        }
        printf("The top %d words (out of %d) are:\n", topWordNum, numWords);
        for (i = 0; i < topWordNum; i++) {
            printf("\t%d %s\n", hashTable[i].frequency, hashTable[i].word);
        }           
    }
    return 0;
}
