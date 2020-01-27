#include "forkit.h"

void forker() {
        if (!(fork())) {
            printf("This is the child, pid %d.\n", getpid());
        } else {
            printf("This is the parent, pid %d.\n", getpid());
            wait(0);
            printf("This is the parent, pid %d, signing off.\n", getpid());
    }
}

int main(int argc, char *argv[]) {
    printf("Hello, world!\n");
    forker();
    return 0;
}    
