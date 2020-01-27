#include <stdio.h>
#include <string.h>

int main() {
    char *word = "the\0";
    char *word2;
    char *token;
    char *token2;
    word2 = strdup(word);
    token = strtok(word2, "h");
  /*  printf("token: %c\n", *token);*/
    if (!token) {
        printf("yes\n");
    }
    printf("word: %s\n", word);
    return 0;
}
    
