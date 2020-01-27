#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *strpbrk(const char *s, const char *accept) {
    char *pr; 
    pr = &s;
    while (1) {
        if (*pr == *accept) {
            
            printf("%c", *pr);
            return *pr;
        } else if (*pr == '\0') {
            return NULL;
        }
        pr++;
    }
    printf("%c", *pr);
}

int main(int argc, const char* argv[]) {
    char *s = "gabe";
    const char *accept = 'e';
    strpbrk(s, accept);
    return 0;
} 
