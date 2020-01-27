#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *file;
    char buff[20];
    file = fopen(argv[1], "r");
    while (fgets(buff, 20, file)) {
        printf("%s", buff);
        memset(buff, 0, 20);
    }
    return 0;
}
