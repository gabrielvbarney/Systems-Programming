#ifndef TIMEIT_H
#define TIMEIT_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>

static int decider = 0; /*global that decides whether to tick or tock*/

void handler(int num);

#endif
