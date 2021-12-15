#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//Size of buffer
#define MAX_SIZE 1024

int mywrite(const int fd_to, char* buffer, int read_size);
int mycopy(const int fd_from, const int fd_to);
