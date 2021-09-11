#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

#define BIGNUMBER 1000000;

struct _process
{
	long int id;
	char* name;
};

typedef struct _process process;
/*
void inf_user(char const * name)
{
	struct passwd* password;
	gid_t* groups = (gid_t*)malloc(32);
	int numofgroups = BIGNUMBER;
	if(password = getpwnam(name))
	{
		printf("uid=%d(%s) ", (int)password->pw_uid, password->pw_name);
		printf("gid=%d(%s) ", (int)password->pw_gid, password->pw_name);
		printf("groups=%d(%s)", (int)password->pw_gid, password->pw_name);
		numofgroups = getgrouplist(name, password->pw_gid, groups, &numofgroups);
		for(int i = 1; i < numofgroups; i++)
		{
			printf(",%d(%s)", groups[i], getgrgid(groups[i])->gr_name);
		}
	}
	else
		printf("id: '%s': no such user", name);
}
*/

process* inf_all(int* elems)
{
	gid_t* grouplist = (gid_t*)malloc(1024);
	int numofgroups = getgroups(1024, grouplist);
	process* list = (process*)calloc(numofgroups + 3, sizeof(process));
	struct passwd* pwd;
	pwd = getpwuid(getuid());
	list[0].id = (long int)pwd->pw_uid;
	list[0].name = pwd->pw_name;
	pwd = getpwuid(getgid());
	list[1].id = (long int)pwd->pw_gid;
	list[1].name = pwd->pw_name;
	struct group* gr;
	gr = getgrgid(grouplist[numofgroups-1]);
	list[2].id = (long int)gr->gr_gid;
	list[2].name = gr->gr_name;
	for(int i = 0; i < numofgroups - 1; i++)
	{
		gr = getgrgid(grouplist[i]);
		list[i+3].id = gr->gr_gid;
		list[i+3].name = gr->gr_name;
	}
	printf("####%s####",list[2].name);
	*elems = numofgroups + 2;
	return list;
}

void print(process* output, int status, int elems)
{
	switch(status)
	{
		case 1: printf("uid=%ld(%s) ", output[0].id, output[0].name);
				printf("gid=%ld(%s) ", output[1].id, output[1].name);
				printf("groups=%ld(%s)", output[2].id, output[2].name);
				for(int i = 3; i < elems + 1; i++)
				{
					printf(",%ld(%s)", output[i].id, output[i].name);
				}
				break;
		case 2:
				break;
		default: printf("Something went wrong");
				break;
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	process* output;
	int elems = 0;
	switch(argc)
	{
		case 1: output = inf_all(&elems);
				break;
		case 2:// inf_user(argv[1]);
				break;
		default: printf("Please input only one parameter");
				break;
	}
	print(output, argc, elems);
	return 0;
}