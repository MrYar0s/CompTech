//get N from stdin
//and gain N childs
//ppid and pid
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int myfork(long int n_proc)
{
	int status = 0;
	for(int i = 0; i < n_proc; i++)
	{
		pid_t id = fork();
		if(id < 0)
		{
			perror("Bad child create");
			return -1;
		} else if(id == 0)
		{
			printf("Parent pid = %d, Child pid = %d\n", (int)getppid(), (int)getpid());
		} else
		{
			wait(&status);
			return 0;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	if(argc < 2 || argc > 2)
	{
		perror("Wrong input from console");
		return -1;
	}
	pid_t parent = getpid();
	printf("Process pid = %d\n", (int)parent);
	long int num_process = atoi(argv[1]);
	myfork(num_process);

	return 0;
}