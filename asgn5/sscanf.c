/*C program t illustrate scanf statement */
#include <stdio.h> 
#include <stdlib.h> 


int main () { 
    /*declaring array s */
    char s [] = "3 red balls 2 blue balls";  
    char str [10],str2 [10]; 
    int i; 
    void *a, *b = NULL; 
     /* %*s is used to skip a word */
    sscanf (s,"%d %*s %*s %*s %s %s", &i, str, str2); 
                
    printf ("%d %s %s \n", i, str, str2);

    if(!a || !b){
        printf("True\n");
    }
    else{ printf("False\n");}
    return 0; 
} 
