#include <string.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	char* b = argv[1];
	char* a = argv[2];
	strcpy(a, b);
	printf("b = %s\n", b);
	printf("a = %s\n", a);
	printf("b = %p, a = %p", b, a);
	return 0;
}