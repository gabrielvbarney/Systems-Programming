/*  Gabriel Barney  */

#include <stdio.h>

int main(int argc, char *argv[]) {
    
    int ch;
    int counter = 1;
    int totalCount = 0;    /* count of chars on row */
    int isTabPrev = 0;     /* is a 1 if tab was the previous input */

    while((ch = getchar()) != EOF) {
        
        if (ch == '\t') {
            isTabPrev = 1;
            while (counter < 9) {
                putchar(' ');
                counter++;
                totalCount++;
            }
            counter = 1;
        }
        else if (ch == '\b') {
            if (isTabPrev == 1) {
                totalCount = totalCount - 8;
                putchar(ch);
                counter = 1;
            }
            else if (totalCount == 0) {      
                putchar(ch);
            } 
            else {
                counter--;
                totalCount--;
                putchar(ch);
            }
        }
        else if (ch == '\n' || ch == '\r') {
            totalCount = 0;       
            counter = 1;
            putchar(ch);
            isTabPrev = 0;
            }
        else {
            putchar(ch);
            totalCount++;
            counter++;
            isTabPrev = 0;
        }
        if (counter > 8 || counter < 1) { 
            counter = 1; 
        }
        if (totalCount < 0) {
            totalCount = 0;
        }
    }
    return 0;
}
