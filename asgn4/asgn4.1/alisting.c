#include "mytar.h"


int list_archive(char *file, uint32_t flags) {
    struct stat info;
    struct passwd *pswd;
    struct group *grp; 
    char _mtime[20];   

/* IN CALLING FUNC, CHECK FOR THE STRICT FLAG CUZ HEADER MORE LOCAL THERE*/

    if (!VFLAG_SET(flags)) {
        printf("%s\n", file);
        return 0;
    }  
   
    if (stat(file, &info)) {
        perror("lstat\n");
        exit(1);
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

    printf("%8ld ", info.st_size);

    strftime(_mtime, 20, "%Y-%m-%d %H:%M", localtime(&(info.st_mtime)));
    printf("%s ", _mtime);
    
    printf("%s", file);
    printf("\n");

    return 0;
}

/*
 * if (argc > 3) {
 *      fdArc = open(argv[2], O_RDONLY);
 *      for (i = 3; i < argc; i++) {
 *          fd = open(argv[i], O_RDONLY);
 *          read(fd, &head, 500);
 *          if (SFLAG_SET(flags)) {  RILEY LOOK: think it goes here
 *              while (head.magic) {
 *                  magic_count += 1; RILEY LOOK: work for bytes?
 *              }
 *              if (magic_count != 5) {
 *                  goes to GNU
 *              }
 *              if (head.mode) {
 *                  goes to GNU 
 *                      or
 *                  Malformed header found.  Bailing.                  
 *              }
 *          }   
 *          lseek(fd, 12, SEEK_CUR);
 *          if (fd == -1) {
 *              perror("%s", argv[i]);
 *              continue;
 *          }
 *          get_name(head.name, head.prefix, name);
 *
 *          while (name != argv[i]) {
 *              lseek(fd, octal_to_decimal(head.size), SEEK_CUR);
 *              memset(&head, 0, 512);
 *              read(fd, &head, 512);
 *              get_name(head.name, head.prefix, name);
 *          }
 *          list_archive(name, flags);
 *          if (lseek(fd, 0, SEEK_SET) == -1) {
 *              perror("bad seek\n");
 *          }
 *      }
 *      close(fdArc);
 *  } else {
 *      fdArc = open(argv[2], O_RDONLY);
 *      if (lstat((char*)fdArc, &info)) {
 *          perror("lstat\n");
 *          exit(1);
 *      }
 *      byte_size = info.st_size;
 *      byte_size -= 1024;
 *      while (byte_size > 0) {
 *          lseek(fdArc, octal_to_decimal(head.size), SEEK_CUR);
 *          memset(&head, 0 512);
 *          read(fdArc, &head, 512);
 *          if (SFLAG_SET(flags)) {  RILEY LOOK: think it goes here
 *              while (head.magic) {
 *                  magic_count += 1; RILEY LOOK: work for bytes?
 *              }
 *              if (magic_count != 5) {
 *                  goes to GNU
 *              }
 *              if (head.mode) {
 *                  goes to GNU 
 *                      or
 *                  Malformed header found.  Bailing.                  
 *              }
 *          }   
 *          get_name(head.name, head.prefix, name);
 *          list_archive(name, flags);
 *          byte_size -= 512;
 *      }
 *      close(fdArc);
 *  }    
