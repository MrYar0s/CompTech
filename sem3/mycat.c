   //////////////////////////////////
  //            MY CAT            //
 //          VERSION 1.0         //
//////////////////////////////////
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

//Size of buffer
#define MAX_SIZE 1024

//Function to write all symbols from buffer to output
//fd - file desctriptor
//buffer - pointer on array of chars
//read_size - num of chars that we have succesfully read
int mywrite(const int fd, char* buffer, int read_size)
{
	//output_size - num of chars that we have in output
	ssize_t output_size = 0;
	do
	{
		//write_size - num of chars that we succesfully wrote
		ssize_t write_size = write(1, buffer, read_size - output_size);
		//Write check
		if(write_size < 0)
		{
			perror("Write Error");
			return -1;
		}
		//offset on write_size symbols
		buffer += write_size;
		//num of symbols that we have in the output
		output_size += write_size;
	} while(read_size - left_size != 0);
	return 0;
}

//Function to call mywrite and read
//fd - file descriptor
int cat(const int fd)
{
	do
	{
		//buffer - array of chars that we have read from fd
		char buffer[MAX_SIZE];
		//read_size - num of chars that we succesfully read
		ssize_t read_size = read(fd, buffer, MAX_SIZE);
		//Read check
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
	//fd - file descriptor
	int fd = 0;
	//Mode selection
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			//open file and get his fd
			fd = open(argv[i], O_RDONLY);
			//Open check
			if(fd < 0)
			{
				perror(argv[i]);
				return -1;
			}
			if(cat(fd) < 0)
				return -1;
			//Close check
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