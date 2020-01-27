#ifndef HASH_SET_H
#define HASH_SET_H

#define QUAD_LOAD_FACTOR 0.5 /*This is the maximum size of the load for 
                               quadratic probing*/


typedef struct {
    char *word;       /* The word/key associated with the entry */
    int  frequency;   /* The number of times the word occurs */
} HashEntry;

extern void create_hash_table();

extern float get_load_factor();

extern int hash_code(char *word, int tableSize);

extern void insert(char *str);

extern int comparator(const void *a, const void *b);

extern void sort_hash_entries(HashEntry *hashTable);

extern void rehash();

extern void reinsert(char *word, int freq, HashEntry *table);

extern void print_hash_table();

#endif
