#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <assert.h>

#define BIGNUMBER 1000

struct process
{
	long int proc_id;
	long int group_id;
	gid_t* grouplist;
	int ngroups;
	char* proc_name;
	char* group_name;
};

int process_init(struct process* user)
{
	user = (struct process*)calloc(1, sizeof(struct process));
	user->proc_name = (char*)calloc(16, sizeof(char));
	if(!user->proc_name)
	{
		printf("Unsuccesful calloc for process name");
		return 1;
	}
	user->group_name = (char*)calloc(16, sizeof(char));
	if(!user->group_name)
	{
		printf("Unsuccesful calloc for group name");
		return 2;
	}
	user->grouplist = (gid_t*)calloc(32, sizeof(gid_t));
	if(!user->grouplist)
	{
		printf("Unsuccesful calloc for grouplist");
		return 3;
	}
	user->ngroups = 0;
	free(user->proc_name);
	free(user->group_name);
	free(user->grouplist);
	free(user);
	return 0;
}

void print_proc(struct process* user)
{
	printf("proc_id = %ld\nuser_id = %ld\n", user->proc_id, user->group_id);
	for(int i = 0; i < 5; i++)
	{
		printf("gr_id[%d] = %ld\n", i, (long int)user->grouplist[i]);
	}
	printf("ngroups = %d\nproc_name = %s\ngroup_name = %s\n", user->ngroups, user->proc_name, user->group_name);
}

int set_user(struct process* user, struct passwd* pwd)
{
	user->proc_id = (long int)pwd->pw_uid;
	user->proc_name = pwd->pw_name;
	user->group_id = (long int)pwd->pw_gid;
	user->group_name = getgrgid(pwd->pw_gid)->gr_name;
	user->ngroups = BIGNUMBER;
	return 0;
}

int process_info(struct process* user, const char* name)
{
	struct passwd* pwd = getpwnam(name);
	if(!pwd)
	{
		printf("id: '%s': no such user\n", name);
		return 1;
	}
	set_user(user, pwd);
	user->ngroups = getgrouplist(user->proc_name, user->group_id,
								 user->grouplist, &(user->ngroups));
	return 0;
}

int all_info(struct process* user)
{
	struct passwd* pwd = getpwuid(getuid());
	set_user(user, pwd);
	user->ngroups = getgroups(BIGNUMBER, user->grouplist);
	return 0;
}

void print_all(struct process* user)
{
	printf("uid=%ld(%s) ", user->proc_id, user->proc_name);
	printf("gid=%ld(%s) ", user->group_id, user->group_name);
	printf("groups=%ld(%s)", (long int)user->grouplist[user->ngroups - 1],
							  getgrgid(user->grouplist[user->ngroups - 1])->gr_name);
	for(int i = 0; i < user->ngroups - 1; i++)
	{
		printf(",%ld(%s)", (long int)user->grouplist[i],
							getgrgid(user->grouplist[i])->gr_name);
	}
	printf("\n");
}

void print_process(struct process* user)
{
	printf("uid=%ld(%s) ", user->proc_id, user->proc_name);
	printf("gid=%ld(%s) ", user->group_id, user->group_name);
	printf("groups=%ld(%s)", (long int)user->grouplist[0],
							  getgrgid(user->grouplist[0])->gr_name);
	for(int i = 1; i < user->ngroups; i++)
	{
		printf(",%ld(%s)", (long int)user->grouplist[i],
							getgrgid(user->grouplist[i])->gr_name);
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	struct process user;
	if(process_init(&user))
		return 1;
	switch(argc)
	{
		case 1: if(!all_info(&user))
					print_all(&user);
				break;
		case 2: if(!process_info(&user, argv[1]))
					print_process(&user);
				break;
		default: printf("Please input only one parameter");
				break;
	}
	return 0;
}
