#ifndef MYPWD_H
#define MYPWD_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>

#define PATH_MAX 2048
#define CURRENT_DIR "."
#define PARENT_DIR ".."

char thePath[PATH_MAX + 1];

void pathBuilder(struct dirent *direct, int size);

#endif
