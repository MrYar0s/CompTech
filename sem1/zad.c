#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print(int, int*, char const *argv[]);
int isnumber(const char*);

int main(int argc, char const *argv[])
{
	int* nums;
	int i = 0;
	nums = (int*)calloc(argc+1,sizeof(int));
	while (i < argc)
	{
		if(isnumber(argv[i]))
			nums[i] = atoi(argv[i]);
		else
			nums[i] = 1;
		i++;
	}
	print(i, nums, argv);
	return 0;
}

void print(int argc, int* nums, char const *argv[])
{
	int divt = 0;
	int divf = 0;
	int i = 1;
	while (i < argc)
	{
		if (!(nums[i] % 3))
		{
			printf("bizz");
			divt = 1;
		}
		if (!(nums[i] % 5))
		{
			printf("bazz");
			divf = 1;
		}
		if (!divf && !divt)
		{
			printf("%s", argv[i]);
		}
		i++;
		if(i != argc)
			printf(" ");
		divt = 0;
		divf = 0;
	}
	printf("\n");
}

int isnumber(const char* word)
{
	int num = strlen(word);
	for(int i = 0; i < num; i++)
		if(!isdigit(word[i]))
			return 0;
	return 1;
}