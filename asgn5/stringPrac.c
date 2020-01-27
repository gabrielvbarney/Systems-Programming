#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <signal.h>

#define  WORD "DOG"


char *int_to_ascii(int stageNum, char* a){
    

    if(stageNum < 10){
        a[0] = (char) (stageNum + 48);
        a[1] = '\0';
    }

    else{
        a[0] = (char) ((stageNum / 10) + 48);
        a[1] =(char) ((stageNum % 10) + 48);
        a[2] = '\0';

    }
    return a;
}







int main(){
    char str[3] = {'\0'};
    int i = 32;
    char buffer[512];
    char strChr[] = "    Word < word";
    char word[] = "Word ";
    char strTok[] = " Word < dog > boy";
    char output[] = "The baby > dog <  bitch";
    char *s;
    char *t;
    char *r;
    char *dup;
    memset(buffer, 0, 512);
    printf("Buff: %s\n", buffer);
    printf("%s\n", WORD);
    char num[2] = {'\0'};
    strcpy(buffer, strcat(word, int_to_ascii(i, str)));
    printf("%s\n", buffer);
    s = strchr(strChr, '<');
    r = strchr(strChr, 'w');
    printf("Value is: %d\n", (&r - &s));
    s = strtok(strChr, "<");
    t = strtok(strTok, ">");
    t = strtok(t, "<");
    r = strtok(output, ">");
    r = strtok(NULL, ">");
    r = strtok(r, " ");
    dup = strdup(r);
 /*   s = strtok(NULL, "<");*/
    printf("After Tok:\"%s\"\n", s);
    printf("After two fatty tokes: \"%s\"\n", t);
    printf("Get output redirect: \"%s\"\n", r);
    printf("STRDUP: \"%s\"\n", dup);


    return 0;
}
