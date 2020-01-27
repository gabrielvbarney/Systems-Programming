#include <stdio.h>

int main(int argc, char* argv[]) {
    char s[] = "Hello, world!\n";
    char *p;
    for (p = s; *p != '\0'; p++)
        putchar(*p);
    return 0;
}
