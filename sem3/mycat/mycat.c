    //////////////////////////////////
   //            MY CAT            //
  //          VERSION 1.0         //
 //         Made By MrYar0s      //
//////////////////////////////////
#include "mycat.h"

//Function to write all symbols from buffer to output
//fd - file desctriptor
//buffer - pointer on array of chars
//read_size - num of chars that we have succesfully read
int mywrite(const int fd_to, char* buffer, int read_size)
{
	//output_size - num of chars that we have in output
	ssize_t output_size = 0;
	do
	{
		//write_size - num of chars that we succesfully wrote
		ssize_t write_size = write(fd_to, buffer, read_size - output_size);
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
	} while(read_size - output_size != 0);
	return 0;
}

//Function to call mywrite and read
//fd - file descriptor
int mycopy(const int fd_from, const int fd_to)
{
	//read_size - num of chars that we succesfully read
	ssize_t read_size = 0;
	do
	{
		//buffer - array of chars that we have read from fd
		char buffer[MAX_SIZE];
		read_size = read(fd_from, buffer, MAX_SIZE);
		//Read check
		if(read_size < 0)
		{
			perror("Read Error");
			return -1;
		}
		if(mywrite(fd_to, buffer, read_size) < 0)
			return -1;
	} while(read_size);
	close(fd_to);
	close(fd_from);
	return 0;
}