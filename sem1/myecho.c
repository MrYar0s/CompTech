#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int nl = 1;
	int i = 1;
	if(argc > 1 && !strcmp(argv[1],"-n"))
	{
		nl = 0;
		i = 2;
	}
	while(i < argc)
	{
		printf("%s", argv[i]);
		i++;
		if(i != argc)
			printf(" ");
	}
	if(nl)
		printf("\n");
	return 0;
}