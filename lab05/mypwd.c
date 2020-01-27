#include "mypwd.h"

int main(int argc, char* argv[]) {
    struct dirent *direct;
    DIR *currDir;
    int errDetect;
    int rootFlag = 0;
    struct stat *dirInfo = (struct stat*)malloc(sizeof(struct stat));
    struct stat *tempDir = (struct stat*)malloc(sizeof(struct stat));    

    if (argc != 1) {
        fprintf(stderr, "usage: ./a.out\n");
        exit(1);
    }
    errDetect = lstat(CURRENT_DIR, dirInfo);
    while (!rootFlag) {
        chdir(PARENT_DIR);
        currDir = opendir(CURRENT_DIR);
        while((direct = readdir(currDir))) {
            errDetect = lstat(direct->d_name, tempDir);
            if (errDetect) {
                perror("mypwd\n");
                exit(1);
            }
            if (tempDir->st_ino == dirInfo->st_ino) {
                if (tempDir->st_dev == dirInfo->st_dev) {
                    if (tempDir->st_mode == dirInfo->st_mode) {
                        if (*direct->d_name == '.') {
                            rootFlag = 1;
                            closedir(currDir);
                            break;
                        }
                        pathBuilder(direct, (int)strlen(direct->d_name));
                        errDetect = lstat(CURRENT_DIR, dirInfo);
                        closedir(currDir);
                        break;
                    }
                }
            }
        }   
    }
    printf("%s\n", thePath);     
    free(dirInfo);
    free(tempDir); 
    return 0;
}

void pathBuilder(struct dirent *direct, int size) {
    int currSize = 0;
    int i = 0;
    while (thePath[currSize]) {
        currSize++;
    }
    while (currSize + 1) {
        thePath[currSize + size + 1] = thePath[currSize];
        currSize--;
    }
    for (i = 0; i <= size; i++) {
        if (i == 0) {
            thePath[i] = '/';
        } else {
            thePath[i] = direct->d_name[i - 1];
        }
    }       
}
