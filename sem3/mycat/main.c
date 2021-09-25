#include "mycat.h"
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
			if(mycopy(fd, 1) < 0)
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
		if(mycopy(fd, 1) < 0)
			return -1;
	return 0;
}