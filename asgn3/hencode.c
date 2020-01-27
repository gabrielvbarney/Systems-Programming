#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "htable.h"

int main(int argc, char* argv[]) {
    int rfile;
    int wfile;
    FILE *fp;
    char c;
    uint32_t bit = NULL;

    
    if (argc > 1) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            fprintf(stderr, "usage: hencode infile [ outfile ]\n");
            exit(1);
        }
        if (argc == 3) {
            wfile = open(argv[2], O_WRONLY | O_CREAT, 0666);
        }
    } else {
        fprintf(stderr, "usage: hencode infile [ outfile ]\n");
    }
    if ((c = getc(fp)) == EOF) {
        if (argc == 3) {
            write(wfile, &bit, sizeof(bit));
            close(wfile);
            exit(0);
        } else {
            write(STDOUT_FILENO, &bit, sizeof(bit));
            exit(0);
        }
    } 
    return 0;

}
     
