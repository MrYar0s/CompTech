#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

#define BIGNUMBER 1000000;

void inf_all(void);
void inf_user(char const *);

int main(int argc, char const *argv[])
{
	if(argc == 2)
	{
		inf_user(argv[1]);
	}
	else if(argc == 1)
	{
		inf_all();
	} else
		printf("Error input");
	printf("\n");
	return 0;
}

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

void inf_all(void)
{
	struct group* gr;
	gid_t* grouplist;
	uid_t userid = getuid();
	uid_t groupid = getgid();
	grouplist = (gid_t*)malloc(32);
	int numofgroups = getgroups(32, grouplist);
	gr = getgrgid(grouplist[numofgroups - 1]);
	printf("uid=%d(%s) ", (int)userid, getpwuid(userid)->pw_name);
	printf("gid=%d(%s) ", (int)groupid, gr->gr_name);
	printf("groups=%d(%s)", (int)gr->gr_gid, gr->gr_name);
	for(int i = 0; i < numofgroups - 1; i++)
	{
		gr = getgrgid(grouplist[i]);
		printf(",%d(%s)", gr->gr_gid,gr->gr_name);
	}
}