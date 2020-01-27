/* Name: Gabriel Barney */

#include <stdio.h>
int printNums();
int printLast();

int main(int argc, char* argv[]) {
    printNums();
    printLast();
    return 0;
}

int printNums() {
    int i;
    for (i = 0; i < 10; i++) {
        printf("%i\n", i);
    }
    return 0;
}

int printLast() {
    char answer[20];
    printf("Cats or Dogs? (c/d): ");
    scanf("%s", answer);
    if (answer[0] == 'C' || answer[0] == 'c') 
        printf("meow.\n");
    else if (answer[0] == 'D' || answer[0] == 'd')
        printf("bork bork.\n");
    else
        printf("CATS OR DOGS. RUN THE PROGRAM AGAIN.\n");
    return 0;
}
    
