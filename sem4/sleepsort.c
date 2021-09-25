#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int myfork(long int n_proc, int* numbers)
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
			usleep(1500*numbers[i]);
			printf("%d ", numbers[i]);
			return 0;
		}
	}
	for(int i = 0; i < n_proc; i++)
		wait(&status);
	printf("\n");
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		perror("Wrong input");
		return -1;
	}
	int num_process = argc - 1;
	int* numbers = (int*) calloc(num_process, sizeof(int));
	for(int i = 0; i < num_process; i++)
		numbers[i] = atoi(argv[i+1]);
	myfork(num_process, numbers);
	free(numbers);
	return 0;
}