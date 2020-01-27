#include "mytar.h"

int main(int argc, char *argv[]) {
    int argCheck;
    char cwd[SIZE];
    uint8_t flags;
    int fd;
    int magic_count;
    off_t byte_size;
    struct stat info;
    off_t bytes_read;
    int non_dir_flag = 0;
    int tester = 0;

    char buffer[500] = {0};
    int i = 0;
    struct header head;
    uint8_t name[255] = {0};

    if (!getcwd(cwd, sizeof(cwd))) {
        fprintf(stderr, "getcwd() error()");
        exit(1);
    }

    /* fd = open(argv[2], (O_WRONLY | O_RDONLY | O_TRUNC | O_CREAT,
             S_IRWXU | S_IRWXG | S_IRWXO);*/

    /* if(fd == -1){
         perror("bad file.... bad\n");
         exit(1); * I think we should exit not sure *
     }*/

    argCheck = argschecker(argc, argv, cwd);

    if (argCheck) {
        exit(1);
    }

    flags = modeCheck(argv[1]);

    if (!FFLAG_SET(flags)) {
        fprintf(stderr, 
"Bad addres\nwrite: Bad file descriptor\nBad file descriptor\n"
        );
        exit(-1);
    }


    if(CFLAG_SET(flags)){
        fd = open(argv[2], O_WRONLY | O_RDONLY | O_TRUNC | O_CREAT,
                  S_IRWXU | S_IRWXG | S_IRWXO);
        if(fd == -1){
            fprintf(stderr, "bad file.... bad\n");
            exit(1);
        }
        create_archive(fd,argv[3],flags);
        /* write(fd, buffer, 512);
         write(fd, buffer, 512);*/
        close(fd);
    }

    if(XFLAG_SET(flags)){
        if(argc == 3){
            extract_archive(argv, flags);
        }
        else{
            for(i = 3; i < argc; i++){
                if((fd = open(argv[2], O_RDONLY,
                              S_IRUSR | S_IRGRP | S_IROTH) == -1)){
                    perror(argv[2]);
                    exit(1);

                }
                read(fd, &head, 500);
                lseek(fd, 12, SEEK_CUR);
                get_name(head.name, head.prefix, name);
                while((char *)name != argv[i]){
                    lseek(fd, octal_to_decimal(head.size, 12), SEEK_CUR);
                    memset(&head, 0, 500);
                    read(fd, &head, 500);
                    lseek(fd, 12, SEEK_CUR);
                    memset(name, 0, 255);
                    get_name(head.name, head.prefix, name);
                }
                extract_specific_file(head, flags, fd);
                if(lseek(fd, 0, SEEK_SET) == -1){
                    fprintf(stderr, "bad lseek... you naughty boy ;)");
                }

            }
            close(fd);
        }
    }
    if (TFLAG_SET(flags)) {
        if (argc > 3) {
            if((fd = open(argv[2], O_RDONLY,
                        (S_IRUSR | S_IRGRP | S_IROTH))) == -1){
                    perror(argv[2]);
                }
            for (i = 3; i < argc; i++) {
                memset(buffer, 0, 500);
                read(fd, buffer, 500);
                lseek(fd, 12, SEEK_CUR);
                if (SFLAG_SET(flags)) {
                    while (head.magic) {
                        magic_count += 1;
                    }
                    if (magic_count != 5) {
                        /*   goes to GNU*/
                    }
                    magic_count = 0;
                    if (head.mode) {
                        /* goes to GNU
                            or
                        Malformed header found.  Bailing.      */
                    }
                }
               /* lseek(fd, 12, SEEK_CUR);*/
                /* figure out for directory 
 * buffer to store directory
 * slash at end
 * if directory append to og
 * else just add file */
                if (lstat(buffer, &info)) {
                }
                while (strcmp(buffer, argv[i])) {
                    if (!S_ISDIR(info.st_mode)) {
               /*    lseek(fd, octal_to_decimal((uint8_t *)(buffer+124),
 *    12), SEEK_CUR);*/
                }
                    memset(buffer, 0, 500);
                    read(fd, buffer, 500);
                    lseek(fd, 12, SEEK_CUR);
                    /*printf("buffer: %s | argv[i]: %s", buffer, argv[i]);*/
                
                }
                
                list_archive(buffer, flags);
    
                
            }
            close(fd);
        }
        else {
            if((fd = open(argv[2], O_RDONLY,
                          (S_IRUSR | S_IRGRP | S_IROTH))) == -1){
                fprintf(stderr, "%s\n", argv[2]);
            }
            if (lstat(argv[2], &info)) {
                fprintf(stderr, "lstat error\n");
              /*  exit(1);*/
            }
            byte_size = info.st_size;
            byte_size -= 2048;
            bytes_read = 0;
            while (bytes_read < byte_size) {
              /*  if((fd = open(argv[2], O_RDONLY,
                          (S_IRUSR | S_IRGRP | S_IROTH))) == -1){
                    fprintf(stderr, "%s\n", argv[2]);
                }*/
                memset(buffer, 0, 500);
                read(fd, buffer, 500);
                lseek(fd, 12, SEEK_CUR);
                if (SFLAG_SET(flags)) {
                    while (head.magic) {
                        magic_count += 1;
                    }
                    if (magic_count != 5) {
                        /*        goes to GNU*/
                    }
                    if (head.mode) {
                        /*  goes to GNU
                              or
                          Malformed header found.  Bailing.  */
                    }
                }
            /*    if (non_dir_flag) {
                    non_dir_flag = 0;
                    continue;
                }*/
                list_archive(buffer, flags);
                if (lstat(buffer, &info)) {
                    fprintf(stderr, "new lstat\n");
                }
                if (!S_ISDIR(info.st_mode)) {
                 /*   non_dir_flag = 1;*/
                   /* bytes_read += info.st_size;*/ /*does not work outside bc 
directories*/
        lseek(fd, octal_to_decimal((uint8_t *)(buffer + 124), 12), SEEK_CUR);
                }
                 
              /*  printf("bytes_read: %lu | byte_size: %lu\n", bytes_read, 
 *  byte_size);*/
            }
            close(fd);
        
        }
   } 
    return 0;
}




uint8_t modeCheck(char *input) {
    int i;
    uint8_t flags = 0;
    for (i = 0; i < strlen(input); i++) {
        switch (input[i]) {
            case 'c':
            flags |= CFLAG;
            break;

            case 't':
            flags |= TFLAG;
            break;

            case 'x':
            flags |= XFLAG;
            break;

            case 'v':
            flags |= VFLAG;
            break;

            case 'f':
            flags |= FFLAG;
            break;

            case 'S':
            flags |= SFLAG;
            break;

            default:
            break;

        }
    }
    return flags;
}


int argschecker(int argc, char *argv[], char cwd[]) {
    int i = 0;
    int ctxFlag = 0;
    int badSyms_i = 0;
    int flexSize = SIZE;
    char *badSyms = (char*)malloc(sizeof(char)*flexSize);

    if (argc == 1) {
        fprintf(stderr, 
"/%s: you must specify at least one of the 'ctx' options.\n", cwd);
        fprintf(stderr, 
"usage: /%s [ctxvS[f tarfile]] [file1 [ file1 [ file2 [...] ] ]\n", cwd);
        return 1;
    }

    if (argc >= 2) {
        for (i = 0; i < strlen(argv[1]); i++) {
           if ((argv[1][i] != 'c') && (argv[1][i] != 't') &&
                   (argv[1][i] != 'x')) {
                if ((argv[1][i] != 'v') && (argv[1][i] != 'f')
                        && (argv[1][i] != 'S')) {
                    if (badSyms_i == SIZE) {
                        flexSize += SIZE;
                        badSyms = realloc(badSyms, flexSize);
                    }
                    badSyms[badSyms_i] = argv[1][i];
                    badSyms_i++;
                }
            }
           else {
                ctxFlag++;
            }
        }
        if (badSyms) {
            for (i = 0; i < badSyms_i; i++) {
                fprintf(stderr, 
"/%s/mytar: unrecognized option '%c'.\n", cwd, badSyms[i]);
            }
        }
        if (!ctxFlag) {
            fprintf(stderr, 
"/%s/mytar: you must choose one of the 'ctx' options.\n", cwd);
        }
        if (ctxFlag > 1) {
            fprintf(stderr, 
"/%s/mytar: you may only choose one of the 'ctx' options.\n", cwd);
        }
        if (!ctxFlag || badSyms_i || ctxFlag > 1) {
            fprintf(stderr, 
"usage: /%s/mytar [ctxvS[f tarfile]] [file1 [ file1 [ file2 [...] ] ]\n", cwd);
            return 1;
        }
    }

    free(badSyms);
    return 0;
}

void build_header(char *path, int wfd) {
    struct header head;
    struct stat info;
    struct passwd *pwd;
    struct group *grp;
    char buffer[150] = {0};
    int link_size;
    char filler[12];
    memset(&head, 0, sizeof(head));
    if(lstat(path, &info) != 0){
    /*    perror("lstat %s error", path); */
        exit(1);
    }
    if (strlen(path) > 99) {
        memcpy(head.name, path, 100);
        memcpy(head.prefix, (char *) ((uintptr_t) path + 100),
               strlen(path) - 100);
    }
    else {
        memcpy(head.name, path, strlen(path));
    }

    decimalToOctal((info.st_mode &
                (S_ISUID|S_ISGID|S_ISVTX|S_IRUSR|S_IWUSR|
                 S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH)),
            (char *)head.mode, 8);
  /*  printf("Mode: 0x%04x, %d\n", info.st_mode, info.st_mode);
    printf("%s\n", (char *)head.mode); */
    decimalToOctal(info.st_uid, (char *)head.uid, 8);
    /*printf("UID: 0x%04x, %d\n", info.st_uid, info.st_uid);
    printf("%s\n", (char *)head.uid); */
    decimalToOctal(info.st_gid, (char *)head.gid, 8);
   /* printf("GID: 0x%04x, %d\n", info.st_gid, info.st_gid);
    printf("%s\n", (char *)head.gid); */
    decimalToOctal(info.st_size, (char *)head.size, 12);
    decimalToOctal(info.st_mtime, (char *)head.mtime, 12);
    /*printf("MTIME: 0x%04x, %d\n", info.st_mtime, info.st_mtime);*/
    /*printf("%s\n", (char *)head.mtime); */


    if (S_ISREG(info.st_mode) != 0) {
        head.typeflag = '0';
    }

    else if (S_ISLNK(info.st_mode) != 0) {
        head.typeflag = '2';
    /*    strcpy((char*)head.size, "00000000000");*/
        memset(head.linkname, 0, 100);
        link_size = readlink(path, buffer, 101); /* for potential null */
        
        if (link_size == -1) {
            perror(path); /*return -1 on error*/
            exit(1);
        }
      /*  else if (link_size > 100) {  changed from 100 + 1 to 100 
            fprintf(stderr, "%s: has to many characters", path);
        }*/
      /*  else {
            memcpy((char *)head.linkname, link_buf, 101);
        }*/
        buffer[strlen(buffer)] = '\0'; /*terminate at the end of the string*/
        strncpy((char*)&head.linkname, buffer, 100);
                                 
       /* memcpy(&head+157, head.linkname, 100);*/

    }
    else if (S_ISDIR(info.st_mode) != 0) {
        head.typeflag = '5';
    }
    else if(S_ISCHR(info.st_mode) || 
S_ISBLK(info.st_mode) || S_ISFIFO(info.st_mode)){
/*else if ( S_ISCHR(info.st_mode)S_ISBLK(info.st_mode)S_ISFIFO(info.st_mode))*/
        head.typeflag = '\0';
    }

   
    memcpy((char *)head.magic, "ustar", 6);
    memcpy((char *)head.version, "00", 2);

    if ((pwd = getpwuid(info.st_uid))) {
        if (strlen(pwd->pw_name) < 32) {
            /*this means the name fits */
            strncpy((char *)head.uname, pwd->pw_name, 32);
        }
        else {
            /* its to long so we need to truncate this bitty */
            memcpy((char *)head.uname, path, 32);
        }
    }
    /* else{
    throw error
    } */

    if ((grp = getgrgid(info.st_gid))) {
        if (strlen(grp->gr_name) < 32) {
            /*this means the name fits */
            strncpy((char *)head.gname, grp->gr_name, 32);
        }
        else {
            memcpy((char *)head.gname, path, 32);
        }
    }
    /* else{
          throw error;
    } */

    /*
    decimalToOctal(major(info.st_rdev), (char *)head.devmajor, 8);
    decimalToOctal(minor(info.st_rdev), (char *)head.devminor, 8);
    */

    /*checksum is last
    loop through every byte, add to checksum*/

    decimalToOctal(checksum_size(head), (char *)head.chksum, 8);

    write(wfd, &head, sizeof(head));

    memset(filler, 0, sizeof(filler));
    write(wfd, filler, sizeof(filler));

}

void decimalToOctal(uint32_t num, char *where, int size){
    uint8_t pack = 0;
    int i = size - 1; /* Start at end of the array so we can do it in reverse*/
    int j = 0;
    char octal_tbl[8] = {'0','1','2','3','4','5','6','7'};
    char *buffer;
    uint32_t max_val = 0;
    int len = 0;
    int t = 0;

    if(size == 8){
        max_val = 2097151;
    }

    if(num > max_val && size == 8){
        if(insert_special_int(where, size, (int32_t)num) != 0){
            perror("special int");
        }
    }
    else {
        buffer = (char *)malloc(size + 1);
        while (num != 0) {
            pack = num & 0x7; /* & with 0111 to check the last three bits */
            num >>= 3;        /* Right shift  3 spaces 
to isolate the next 3 bits*/
            buffer[i] = octal_tbl[pack];
            i--;
            len++;
        }
        i++; /* to account for the last decrement of i */
        
        while(size-len++ != 0){
            where[t++] = '0';
        }
        
        for (j = (strlen(where)-1); i < (size + 2); i++) {
            where[j++] = buffer[i];
        }
        free(buffer);
        where[j] = '\0';
      /*  len = strlen(where);
        j=0;
        while(len < size){
            j++;
        }
        leadingzeros[j] = '\0';
        strcat*/
        
    }

            
       

    
}

uint32_t checksum_size(struct header head){
    uint32_t sum = 0;
    uint32_t i;
    uint8_t *head_ptr = (uint8_t *)(&head);
    for(i = 0; i < 148; i++){
        sum += head_ptr[i];
        /* this gets us the some of every char in the header */
    }
    for(i = 148; i < 156; i++){
        sum += ' '; /* chksum is treated as if its filled with spaces */
    }
    for(i = 156; i < 512; i++){
        sum += head_ptr[i];
    }
    return sum;

}

/* THIS WAS IN THE SPEC SO I ASSUME THAT WE CAN USE IT */
int insert_special_int(char *where, size_t size, int32_t val){
/* set the high–order bit of the first byte, then
* treat the rest of the field as a binary integer
* in network byte order.
* Insert the given integer into the given field
* using this technique. Returns 0 on success, nonzero * otherwise
*/
    int err=0;
    if(val<0||(size<sizeof(val)) ){
/* if it’s negative, bit 31 is set and we can’t use the flag
          * if len is too small, we can’t write it. * done.
*/
    err++;
    }
    else {
    /* game on....*/
    memset(where, 0, size);
    *(int32_t *)(where + size - sizeof(val)) = htonl(val); /* place the int */
    *where |= 0x80; /* set that high–order bit */
    }
    return err;
}


/* open file
 * check if file
 *      if file, we write the header the we write contents of the file
 * check if directory
 *      if dir-> open dir get all entries
 *      check all entries
 *      write all entries to arcieve
 *
 */

/* directory file contains directory entries
 * entries contains information
 * on a block, they contain a path that maps to inode
 * inode actually contains the data
 */

/* Open directory and write it
 * open all the shit and write it all
 */

void create_archive(int fdArc, char *path, uint8_t flags){
    struct stat info;
    
    int fd;
    uint32_t bytes_read;
    char buffer[512] = {0};
    DIR *dir;
    struct dirent *dirEntry;

    /* might need to check for permissions,
     * if we own it we can write (user id matches)
     * our group id matches their id
     * if other has RD WR then we have RD WR */


    if(lstat(path, &info) != 0){
        perror("lstat %s error\n");
        /*exit or next file?*/
    }

    if(VFLAG_SET(flags) != 0){
        printf("%s\n",path);
        /* if v is set, print contents as we add it to archive */
    }

    if ((S_ISREG(info.st_mode) != 0) || (S_ISCHR(info.st_mode) !=0) ||
       (S_ISFIFO(info.st_mode) !=0) || (S_ISBLK(info.st_mode) !=0)){
        build_header(path, fdArc); /* write the header */
        /* might need to check for which permissions are chillen*/
        fd = open(path, O_RDONLY);
        if(fd == -1){
            perror(path);
            /*don't think we exit here, might want to continue, possible brk*/
        }
        while((bytes_read = read(fd, buffer, 512)) == 512){
            write(fdArc, buffer, 512);
            memset(buffer, 0, 512);

        }
        if (bytes_read > 0) {
            write(fdArc, buffer, 512);
        }

        memset(buffer, 0, 512);
         write(fdArc, buffer, 512);
         write(fdArc, buffer, 512);  
         close(fd);  /* closing in main*/
    }

    else if (S_ISLNK(info.st_mode) != 0) {
        fd = open(path, O_RDONLY);
        if(fd == -1){
            perror(path);
            /*don't think we exit here, might want to continue, possible brk*/
        }
        build_header(path, fd);
        close(fd); 
    }
    else if (S_ISDIR(info.st_mode) != 0) {
        dir = opendir(path);
        if(!dir){
            perror(NULL);
            exit(1);
        }
      /*  if(dirEntry->d_name == "."){
                chdir("..");
            }*/

        while((dirEntry = readdir(dir)) != NULL){
           /* if(dirEntry->d_name == "."){
                chdir("..");
                if(dirEntry = readdir(".")== NULL){
                    break;
                }
            }*/
            create_archive(fdArc, dirEntry -> d_name, flags);
        }
    }
    

}

/*extract_archive(){
    open file to extract
    read in eader
    if dir make dir
    if file make reg file
    we know what file has to be
    so parse and make exact file
    sprintf
    keep track of size of file reading
    so if dir is 600000, go through all files and make sure 
dont do more than that
    if make dir and make file with path to dir,
}*/

void extract_archive(char *argv[], uint8_t flags){
    struct stat info;
    int fd;
    int newfd; /* this is for the file we need to archive */
    
    off_t size;  
    struct header head;
  /*  char buffer[512] = {0};*/
    uint32_t bytes_read;
    uint8_t name[255] = {0};
    uint32_t fileSize;
    uint32_t mode;
    char *filecontents;
 /* struct passwd *pwd;
    struct group  *grp; */





    /* extract given bc no file names were given to extract
     * extract all decendents if needed */
    if(lstat(argv[2], &info) != 0){
        perror("lstat error\n");
    }
    size = info.st_size;
    size -= 1024; /* this accounts for the two NULL blocks */

    fd = open(argv[2], O_RDONLY);
    if(fd == -1){
        perror(argv[2]);
    }
    while(bytes_read <= size){
        bytes_read += read(fd, &head, 500); /* now we have a header */
        if(lseek(fd, 12, SEEK_CUR) == -1){
            perror("lseek");
        }
        bytes_read += 12;
        get_name(head.name, head.prefix, name); /* now we have file name*/
        if(VFLAG_SET(flags)){
            printf("%s",name);
        }
        fileSize = octal_to_decimal(head.size, sizeof(head.size));
        if(head.typeflag == '0' || head.typeflag == '\0'){
            /* this is a reg or alt file */
            mode = octal_to_decimal(head.mode, sizeof(head.mode)); 
            /* wanted shit to fit */
            newfd = open((char *)name, O_CREAT | O_WRONLY | O_TRUNC, mode);
            /*might want to use chmod?*/
            /* error check? */
            filecontents = (char *)malloc(fileSize); /* maybe size + 1? */
            bytes_read += read(fd, filecontents, fileSize);
            write(newfd, filecontents, fileSize);
            close(newfd);
            lstat((char *)name, &info);
            /*error check right here*/
            /*   info.st_uid = octal_to_decimal(head.uid);
               info.st_gid = octal_to_decimal(head.gid); */
            /* chown does the above */
            chown((char*)name, octal_to_decimal(head.uid, sizeof(head.uid)),
                  octal_to_decimal(head.gid, sizeof(head.gid)));
        /*    pwd = getpwuid(info.st_uid);
            pwd->pw_name = head.uname;
            grp = getgrgid(info.st_gid);
            grp->gr_name = head.gname; */
         /*info.st_mtime = octal_to_decimal(head.mtime, sizeof(head.mtime));*/
            free(filecontents);
        }
        else if(head.typeflag == '2'){
            /* this is a link */
            if(symlink((char*)head.linkname, (char*)name) == 0){
                perror("symlink failure");
               
            }
            if(chown((char*)name, octal_to_decimal(head.uid, sizeof(head.uid)),
                octal_to_decimal(head.gid, sizeof(head.gid))) == -1){
                perror("chownies");
            }

        }
        else if(head.typeflag == '5'){
            /* this is a directory */
            if(mkdir((char*)name, octal_to_decimal(head.mode, 
sizeof(head.mode))) == -1){
                perror("mkdir");
            }
            lstat((char*)name, &info);
            chown((char*)name, octal_to_decimal(head.uid, sizeof(head.uid)),
                  octal_to_decimal(head.gid, sizeof(head.gid)));
   /* info.st_mtime = octal_to_decimal(head.mtime, sizeof(head.mtime));*/

        }
    }
}

void extract_specific_file(struct header head, uint8_t flags, int fdArc){
    /* we want argv[2] -> arc and argv[i] -> the file we extr*/
    struct stat info;
    int newfd; /* this is for the file we need to archive */
    uint8_t name[255] = {0};
    uint32_t mode;
    char *filecontents;
  /*  struct passwd *pwd;
    struct group  *grp;*/

     get_name(head.name, head.prefix, name);
     if(VFLAG_SET(flags)){
        printf("%s", name);
    }
    if(head.typeflag == '0' || head.typeflag == '\0'){
        mode = octal_to_decimal(head.mode, 8); /* wanted shit to fit */
        newfd = open((char*)name, O_CREAT | O_WRONLY | O_TRUNC, mode);
        /*might want to use chmod?*/
        /* error check? */
        filecontents = (char *)malloc(octal_to_decimal(head.size, 12)); 
/* +1?*/
        write(newfd, filecontents, sizeof(filecontents) );

        close(newfd);
        lstat((char *)name, &info);
        /*error check right here*/
        /*   info.st_uid = octal_to_decimal(head.uid);
        info.st_gid = octal_to_decimal(head.gid); */
        /* chown does the above */
        chown((char *)name, octal_to_decimal(head.uid, 8),
              octal_to_decimal(head.gid, 8));
        /* info.st_mtime = octal_to_decimal(head.mtime, 12);*/
        free(filecontents);
    }
    else if(head.typeflag == '2'){
        /* this is a link */
        if(symlink((char *)head.linkname, (char *)name) == 0){
            perror("symping");
        }
        if(chown((char *)name, octal_to_decimal(head.uid, 8),
            octal_to_decimal(head.gid, 8)) == -1){
            perror("chownies");
        }

    }
    else if(head.typeflag == '5'){
        if(mkdir((char *)name, octal_to_decimal(head.mode,
                        sizeof(head.mode))) == -1){
            perror("mkdir");
        }
        lstat((char *)name, &info);
        chown((char *)name, octal_to_decimal(head.uid, 8),
              octal_to_decimal(head.gid, 8));
        /* info.st_mtime = octal_to_decimal(head.mtime, 12);*/
        read(fdArc, &head, 500); /* head of next entry of this directory */
        if(lseek(fdArc, 12, SEEK_CUR) == -1){
            perror("lseek");
        }
        extract_specific_file(head, flags, fdArc);

    }
}

void get_name(uint8_t *name, uint8_t *prefix, uint8_t *dest) {
    int size;
    size = strlen((char *)name);
    strncpy((char *)dest, (char *)name, size);
    size = strlen((char *)prefix);
    if (size == 0) {
        strcat((char *)dest, "/");
    }
    else{
        strncat((char *)dest, (char *)prefix, size);
    }
  /*  dest = (uint8_t *)realloc((char *)dest, strlen((char *)dest));*/
}


uint32_t octal_to_decimal(uint8_t *str, int len){
    uint32_t decimal = 0;
    uint32_t octal = 0;
    uint8_t *newStr = (uint8_t *)malloc(len);
    int i = 0;

    if(str[0] != '0'){ /*octals start with a zero, so if not special int */
        if((decimal = extract_special_int((char *)str, len)) == -1){
            perror("special int");
        }
    }
    else {
        for (i = 1; i < strlen((char *)str)+1; i++) {
            newStr[i - 1] = str[i]; /* this gets rid of the leading zero */
        }

        octal = atoi((char *)newStr);
        i = 0;
        while (octal != 0) {
            decimal += (octal % 10) * pow(8, i++);
            octal /= 10;
        }
    }
    return decimal;

}

uint32_t extract_special_int(char *where, int len) {
/* For interoperability with GNU tar. GNU seems to
* set the high–order bit of the first byte, then
* treat the rest of the field as a binary integer
* in network byte order.
* I don’t know 
* for sure if it’s a 32 or 64–bit int, but for * this version, 
* we’ll only support 32. (well, 31)
* returns the integer on success, –1 on failure.
* In spite of the name of htonl(), it converts int32 t */
    int32_t val= -1;
    if ( (len >= sizeof(val)) && (where[0] & 0x80)) {
/* the top bit is set and we have space * extract the last four bytes */
    val = *(int32_t *)(where + len - sizeof(val));
    val = ntohl(val); /* convert to host byte order */
    }
    return (val);
}

int list_archive(char *file, uint32_t flags) {
    struct stat info;
    struct passwd *pswd;
    struct group *grp;
    char _mtime[20];
    int zero = 0;

/* IN CALLING FUNC, CHECK FOR THE STRICT FLAG CUZ HEADER MORE LOCAL THERE*/

    if (!VFLAG_SET(flags)) {
        printf("%s\n", file);
        return 0;
    }

    if (stat(file, &info)) {
        exit(1);/*possible mem leak*/
      /*  perror("lstat\n");
        printf("%s\n", file);*/
      /*  exit(1);*/
    }

    putchar(S_ISDIR((info.st_mode)) ? 'd' : '-');
    putchar((info.st_mode & S_IRUSR) ? 'r' : '-');
    putchar((info.st_mode & S_IWUSR) ? 'w' : '-');
    putchar((info.st_mode & S_IXUSR) ? 'x' : '-');
    putchar((info.st_mode & S_IRGRP) ? 'r' : '-');
    putchar((info.st_mode & S_IWGRP) ? 'w' : '-');
    putchar((info.st_mode & S_IXGRP) ? 'x' : '-');
    putchar((info.st_mode & S_IROTH) ? 'r' : '-');
    putchar((info.st_mode & S_IWOTH) ? 'w' : '-');
    putchar((info.st_mode & S_IXOTH) ? 'x' : '-');
    putchar(' ');

    pswd = getpwuid(info.st_uid);
    grp = getgrgid(info.st_gid);

    printf("%s/%s ", pswd->pw_name, grp->gr_name);

    if (!S_ISDIR(info.st_mode)) {
        printf("%8ld ", info.st_size);
    }
    else {
        printf("%8d ", zero);
    }

    strftime(_mtime, 20, "%Y-%m-%d %H:%M", localtime(&(info.st_mtime)));
    printf("%s ", _mtime);

    printf("%s", file);
    printf("\n");

    return 0;
}
