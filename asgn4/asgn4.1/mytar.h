#ifndef MYTAR_H
#define MYTAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <byteswap.h>
#include <math.h>

#define SIZE 256
#define CFLAG 0x01
#define TFLAG 0x02
#define XFLAG 0x04
#define VFLAG 0x08
#define FFLAG 0x10
#define SFLAG 0x20

#define CFLAG_SET(f) (f&CFLAG)
#define FFLAG_SET(f) (f&FFLAG)
#define VFLAG_SET(f) (f&VFLAG)
#define TFLAG_SET(f) (f&TFLAG)
#define SFLAG_SET(f) (f&SFLAG)
#define XFLAG_SET(f) (f&XFLAG)




struct __attribute__ ((__packed__)) header {
    uint8_t name[100];
    uint8_t mode[8];
    uint8_t uid[8];
    uint8_t gid[8];
    uint8_t size[12];
    uint8_t mtime[12];
    uint8_t chksum[8];
    uint8_t typeflag;
    uint8_t linkname[100];
    uint8_t magic[6];
    uint8_t version[2];
    uint8_t uname[32];
    uint8_t gname[32];
    uint8_t devmajor[8];
    uint8_t devminor[8];
    uint8_t prefix[155];
};

void build_header(char *path, int wfd);

void decimalToOctal(uint32_t num, char *where, int size);

int argschecker(int argc, char *argv[], char cwd[]);

uint8_t modeCheck(char *input);

uint32_t checksum_size(struct header head);

int list_archive(char *file, uint32_t flags);

int insert_special_int(char *where, size_t size, int32_t val);

void create_archive(int fdArc, char *path, uint8_t flags);

void extract_archive(char *argv[], uint8_t flags);

void get_name(uint8_t *name, uint8_t *prefix, uint8_t *dest);

uint32_t octal_to_decimal(uint8_t *str, int len);

uint32_t extract_special_int(char *where, int len);

void extract_specific_file(struct header head, uint8_t flags, int fdArc); 
#endif
