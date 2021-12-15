#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int param_n = 0;
	int i = 1;
	if(argc > 1 && !strcmp(argv[1],"-n"))
	{
		param_n = 1;
		i = 2;
	}
	while(i < argc)
	{
		printf("%s", argv[i]);
		i++;
		if(i != argc)
			printf(" ");
	}
	if(!param_n)
		printf("\n");
	return 0;
}
