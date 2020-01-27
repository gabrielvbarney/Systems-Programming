#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_set.h"


HashEntry *hashTable;


/*hashTable = NULL; Pointer to an array of HashEntries, starts out at NULL
                     so we know if the hashTable is created or not */
int tableSize = 1013;  
/* We chose to make our table size 4211, becuase 4211 is a prime number. 
 * Having a prime number will make it less likely to have a collision. 
 * In addition, 4211 * 2 + 1 is also prime which is cool*/

int numEntries = 0;

int numWords = 0;

int createdFlag = 0; /* is the table created? if so make 1*/

void create_hash_table(){
    int i;
    hashTable =  (HashEntry *)calloc(tableSize, sizeof(HashEntry));
    /* We chose to calloc because that way the storage is initialized to 0. 
     * This will help us when we need to increase the frequency of a word. */

    if(!hashTable){
        perror("Unable to calloc space for the hashTable");
        exit(1);
    }

    for(i = 0 ; i < tableSize ; i++){
        hashTable[i].word = NULL;
        hashTable[i].frequency = 0;
    }
}

float get_load_factor(){
    return (numEntries / ((float) tableSize));
}

int hash_code(char *word, int tableSize) {
    int hashNum = 0;
    int charLimit = 15; /* max num of characters read for efficiency */
    int primeNum = 31;
    int loopLength = (strlen(word) < charLimit) ? strlen(word) : charLimit;
    int i;
    for (i = 0; i < loopLength; i++) {
        hashNum += (word[i] * pow(primeNum, (loopLength - 1 - i)));
        /* append -lm to end of compile statement to link math.h */ 
    }
    return (hashNum % tableSize);
}



void insert(char *str){
    int index;
    int i; /* used to hold value of original when probing */
    int quad_probe = 1; /* this will be the new index after probing */
    int flag = 0; /* check if we already added an entry in our while loop */
    
    if(createdFlag == 0){
        create_hash_table();
        createdFlag = 1;
    }
    if(!str){
        printf("No str was passed");
        exit(1);
    }
    if(str){
        index = hash_code(str, tableSize); /* make sure str can't be NULL */
    }
    
    if(get_load_factor() > 0.5){
       rehash();
    }
    
    if((hashTable[index].frequency == 0)){
      /*  hashTable[index].word =(char *)malloc(strlen(str) + 1);
        dstrcpy(hashTable[index].word, str); */
        hashTable[index].word = str;
        hashTable[index].frequency++;
        numEntries++;
        numWords++;
    }


    else{
        i = index; 
        while(hashTable[index].frequency > 0){ 
        /* while there is a word at the specified index */
            if((strcmp(hashTable[index].word, str) == 0) && 
              (hashTable[index].word != NULL)){
                hashTable[index].frequency++;
                flag = 1; 
                numWords++;
                break;
            }
            
            index = (quad_probe * quad_probe + i) % tableSize;
            quad_probe++;

        }  
        if(flag == 0){
          /*  hashTable[index].word =(char *)malloc(strlen(str) + 1);
            strcpy(hashTable[index].word, str); */
            hashTable[index].word = str; /* points to the word */ 
            hashTable[index].frequency++;
            numEntries++;
            numWords++; 
        }
    }
}

void reinsert(char *word, int freq, HashEntry *table){
    int index;
    int i;
    int quad_probe = 1;

    index = hash_code(word, tableSize);

    if(get_load_factor() > 0.5){
        rehash();
    }

    if(table[index].frequency == 0){
       /* table[index].word =(char *)malloc(strlen(str) + 1);
        strcpy(hashTable[index].word, str); */
        table[index].word = word;  /* point to the word */
        table[index].frequency = freq;
    }
    else{
        i = index;
        while(table[index].frequency > 0){
           /* quad_probe = pow(numEntries, 2);
            quad_probe += i;
            quad_probe %= tableSize;*/
            index = (quad_probe * quad_probe + i) % tableSize;
            quad_probe++;
            
        }
       /* table[index].word = (char *)malloc(strlen(word) + 1);
        strcpy(table[index].word, str); */
        table[index].word = word; 
        table[index].frequency = freq;
    }

}

int comparator(const void *a, const void *b) {
    char *word1 = ((HashEntry*)a)->word; 
    char *word2 = ((HashEntry*)b)->word;
    int freq1 = ((HashEntry*)a)->frequency;
    int freq2 = ((HashEntry*)b)->frequency;

    if (freq1 != freq2){
        return freq2-freq1;
    }

    if(word1 == NULL && word2 == NULL){
        return 0;
    }
    if (word2 == NULL){
        return -1;
    }
    if (word1 == NULL){
        return 1;
    }
  /*  if (freq1 != freq2){
        return freq2-freq1;*/
    
      
    return strcmp(word2, word1);   
    
}


void sort_hash_entries(HashEntry *hashTable) {
     qsort(hashTable, tableSize, sizeof(HashEntry), comparator);
}

void rehash(){
    HashEntry *tempTable;
    int i;
    int prevSize = tableSize;
    tableSize = (tableSize *2) + 1;
    tempTable = (HashEntry *)calloc(tableSize, sizeof(HashEntry));

    if(!tempTable){
        perror("Unable to calloc space for the tempTable");
        exit(1);
    }    
    for(i = 0 ; i < prevSize ; i++){
        if(hashTable[i].frequency > 0){
            reinsert(hashTable[i].word, hashTable[i].frequency, tempTable);
          /*  free(hashTable[i].word; */ /* using the same address don't free*/
          /*  free(hashTable[i]; */
        }
    }
    free(hashTable);
    hashTable = tempTable; 
    
}

void print_hash_table(){
    int i;
    sort_hash_entries(hashTable);
    for(i = 0; i < tableSize ; i++){
        if(hashTable[i].frequency != 0){
            printf("Word Entry: %s  Word Freq: %d index: %d  numEntries: %d NumUniqueWord: %d \n\n", hashTable[i].word, hashTable[i].frequency, i, numEntries, numWords);
        }
    }
}

