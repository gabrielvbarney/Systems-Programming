#include <stdio.h>
#include <stdlib.h>


char *strfry(char *s) {
    int i;
    int random;
    int randomt;
    int temp;
    char *sp;
    int len = 0;
    if (!s) {
        perror("String is NULL");
        exit(1);
    }
    sp = s;
    while (*sp != '\0') {
        len++;
        sp++;
    }
    for (i = 0; i<len; i++) {
        random = rand() % len;
        randomt = rand() % len;
        temp = s[random];
        s[random] = s[randomt];
        s[randomt] = temp;
    
    }
    for (i = 0; i < len; i++) {
        printf("%c\n", s[i]); 
    }  
    return s;
}


int main(int argc, const char* argv[]) {
    char boof[] = "gabriel";
    strfry(boof);
    return 0;
}
