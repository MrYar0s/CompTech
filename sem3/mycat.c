#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 1024

int mywrite(const int fd, char* buffer, int read_size)
{
	ssize_t left_size = 0;
	do
	{
		ssize_t write_size = write(1, buffer, read_size - left_size);
		if(write_size < 0)
		{
			perror("Write Error");
			return -1;
		}
		buffer += write_size;
		left_size += write_size;
	} while(read_size - left_size != 0);
	return 0;
}

int cat(const int fd)
{
	ssize_t read_size = 0;
	do
	{
		char buffer[MAX_SIZE];
		read_size = read(fd, buffer, MAX_SIZE);
		if(read_size < 0)
		{
			perror("Read Error");
			return -1;
		}
		if(mywrite(fd, buffer, read_size) < 0)
			return -1;
	} while(read_size);
	return 0;
}

int main(int argc, char const *argv[])
{
	int fd = 0;
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			fd = open(argv[i], O_RDONLY);
			if(fd < 0)
			{
				perror(argv[i]);
				return -1;
			}
			if(cat(fd) < 0)
				return -1;
			if(close(fd) < 0)
			{
				perror("Close Error");
				return -1;
			}
		}
	}
	else
		if(cat(fd) < 0)
			return -1;
	return 0;
}