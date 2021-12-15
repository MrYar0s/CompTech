#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>

#define MAX_SIZE 1024

#define USEC 1000000

#define Q_FLAG 1

struct symbols
{
	int nbytes;
	int nlines;
	int nwords;
};

double print_time(struct timeval t1, struct timeval t2)
{
	double usec = ((double)(t2.tv_usec - t1.tv_usec))/USEC;
	double sec = (double)(t2.tv_sec - t1.tv_sec);
	return usec+sec;
}

int mywrite(const int fd_to, char* buffer, int read_size)
{
	ssize_t output_size = 0;
	do
	{
		ssize_t write_size = write(fd_to, buffer, read_size - output_size);
		if(write_size < 0)
		{
			perror("Write Error");
			return -1;
		}
		buffer += write_size;
		output_size += write_size;
	} while(read_size - output_size != 0);
	return 0;
}

int get_opts(int argc, char *argv[])
{
	int flag = 0;
	if(!strcmp(argv[1], "--quiet") || !strcmp(argv[1], "-q"))
		flag |= Q_FLAG;
	return flag;
}

int counter_lines(char* buffer, int size)
{
	int counter = 0;
	for(int i = 0; i < size; i++)
	{
		if(buffer[i] == '\n')
		{
			counter++;
		}
	}
	return counter;
}

int counter_words(char* buffer, int size)
{
	int word_stat = 0;
	int counter = 0;
	for(int c = 0; c < size; c++)
	{
		if(!isspace(buffer[c]) && !word_stat)
		{
			counter++;
			word_stat = 1;
		} else if(isspace(buffer[c]))
		{
			word_stat = 0;
		}
	}
	return counter;
}

int count(int fd, int flag)
{
	struct symbols counter = {0,0,0};
	int bytes_read = 0;
	do
	{
		char buffer[MAX_SIZE];
		bytes_read = read(fd, buffer, MAX_SIZE);
		if (!(flag & Q_FLAG)) {
			mywrite(1, buffer, bytes_read);
		}
		counter.nbytes += bytes_read;
		counter.nlines += counter_lines(buffer, bytes_read);
		counter.nwords += counter_words(buffer, bytes_read);
	} while(bytes_read > 0);
	
	fprintf(stderr, "Num of lines = %d\n", counter.nlines);
	fprintf(stderr, "Num of words = %d\n", counter.nwords);
	fprintf(stderr, "Num of bytes = %d\n", counter.nbytes);
	return 0;
}

int myexec(int argc, char *argv[], int flag)
{
	int fd[2];
	pipe(fd);

	int status;
	pid_t id = fork();

	if(id < 0)
	{
		perror("Wrong process");
		return -1;
	}

	if(id == 0)
	{
		close(fd[0]);
		if(dup2(fd[1], 1) < 0)
		{
			perror("Dup error");
			return -1;
		}
	
		execvp(argv[flag + 1], argv + flag + 1);
		perror("Error with exec");
		return -1;
	}

	if(id > 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		count(0, flag);
		close(fd[0]);
	
		if(wait(&status) < 0)
		{
			perror("Wait error");
			return -1;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		perror("Enter more arguments");
		return -1;
	}

	int flag = get_opts(argc, argv);

	struct timeval t1 = {0, 0};
	struct timeval t2 = {0, 0};
	if(gettimeofday(&t1, NULL) < 0)
	{
		perror("Error with start time");
		return -1;
	}

	if(myexec(argc, argv, flag) < 0)
		return -1;

	if(gettimeofday(&t2, NULL))
	{
		perror("Error with end time");
		return -1;
	}

	printf("\nExecute time = %.3lf\n", print_time(t1,t2));
	return 0;
}
