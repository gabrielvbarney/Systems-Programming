#include "timeit.h"
 
void handler(int num) {
    if (++decider % 2) {
        printf("Tick...");
    } else {
        printf("Tock\n");
    }
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    struct sigaction sa;
    struct itimerval time;
    sigset_t mask;
    int num;
    int i;
    int seconds;

    if (argc != 2) {
        printf("usage: %s <seconds>\n", argv[0]);
        exit(1);
    } else {
        for (i = 0; i < strlen(argv[1]); i++) {
            if (!isdigit(argv[1][i])) {
                printf("%s: malformed time.\n", argv[1]);
                printf("usage: %s <seconds>\n", argv[0]);
                exit(1);
            }
        }
        seconds = atoi(argv[1]);
    }


    sa.sa_handler = handler;
    sa.sa_flags = 0;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigdelset(&mask, SIGINT);
 
    time.it_interval.tv_usec = 500000; /* Half Seconds (1000000 usec/sec) */
    time.it_interval.tv_sec = 0;
    time.it_value.tv_usec = 500000; /* Half Seconds (100000 usec/sec) */
    time.it_value.tv_sec = 0; 

    if (sigaction(SIGALRM, &sa, NULL)) {
        perror("sigaction\n");
        exit(1);
    }
    
    if (setitimer(ITIMER_REAL, &time, NULL)) {
        perror("setitimer\n");
        exit(1);
    }

    num = 0;    
 
    while (num != seconds) {
        sigsuspend(&mask);
        pause();
        num++;
    }
    
    printf("Time's Up!\n");
    return 0;
}
