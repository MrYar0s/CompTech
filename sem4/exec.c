#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>

#define USEC 1000000

double print_time(struct timeval t1, struct timeval t2)
{
	double usec = ((double)(t2.tv_usec - t1.tv_usec))/USEC;
	double sec = (double)(t2.tv_sec - t1.tv_sec);
	return usec+sec;
}

int main(int argc, char *const argv[])
{
	if(argc < 2)
	{
		perror("Wrong input");
		return -1;
	}
	pid_t id = fork();
	int status = 0;
	struct timeval t1 = {0, 0};
	struct timeval t2 = {0, 0};
	if(gettimeofday(&t1, NULL) < 0)
	{
		perror("Error with start time");
		return -1;
	}
	if(id < 0)
	{
		perror("Wrong process");
		return -1;
	} else if (id == 0)
	{
		execvp(argv[1], argv + 1);
		perror("Error with exec");
		return -1;
	} else
	{
		wait(&status);
	}
	if(gettimeofday(&t2, NULL))
	{
		perror("Error with end time");
		return -1;
	}
	printf("\nExecute time = %.3lf\n", print_time(t1,t2));
	return 0;
}
