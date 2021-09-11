#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	unsigned int pid = 0;
	unsigned int ppid = 0;
	pid = (unsigned int)getpid();
	ppid = (unsigned int)getppid();
	printf("your pid = %u, parent pid = %u\n", pid, ppid);
	return 0;
}