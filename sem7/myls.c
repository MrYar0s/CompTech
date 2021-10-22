#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <pwd.h>
#include <time.h>

#define MAX_SIZE 1024

#define L_OPT 1
#define A_OPT 2
#define R_OPT 4
#define I_OPT 8
#define N_OPT 16

#define LONG_OPTS "laRin"
#define MOD_NUM 10

typedef struct option OPT;

OPT options[] =
{
	{"long", 0, NULL, 'l'},
	{"all", 0, NULL, 'a'},
	{"recursive", 0, NULL, 'R'},
	{"inode", 0, NULL, 'i'},
	{"numeric-uid-gid", 0, NULL, 'n'},
	{0, 0, 0, 0}
};

char* concat(char* path, char* name)
{
	char* real_path = (char*)malloc(sizeof(path) + sizeof(name) + 1);
	real_path[0] = '\0';
	strcat(real_path, path);
	strcat(real_path, "/");
	strcat(real_path, name);
	return real_path;
}

void printname(struct dirent* file, struct stat* info)
{
	if((info->st_mode & S_IEXEC) && (info->st_mode & ~S_IFDIR))
	{
		printf("\033[0;32m");
	}
	if(info->st_mode & S_IFDIR)
	{
		printf("\033[0;34m");
	}
	printf("%s ", file->d_name);
	printf("\033[0m");
}

void printrwx(mode_t mode)
{
	char rwx[MOD_NUM] = {};
	if(S_IFDIR & mode)
		rwx[0] = 'd';
	else
		rwx[0] = '-';
	for(int i = 3; i > 0; i--)
	{
		if(1 & mode)
			rwx[3*i] = 'x';
		else
			rwx[3*i] = '-';
		if(2 & mode)
			rwx[3*i-1] = 'w';
		else
			rwx[3*i-1] = '-';
		if(4 & mode)
			rwx[3*i-2] = 'r';
		else
			rwx[3*i-2] = '-';
		mode = mode>>3;
	}
	printf("%s ", rwx);
}

void printdata(struct stat* info)
{
	char* data = (char*)malloc(MAX_SIZE);
	strftime(data, MAX_SIZE, "%B %d %R", localtime(&(info->st_ctime)));
	printf("%s ", data);
	free(data);
}

void printinfo(struct dirent* file, struct stat* info, int options)
{
	if(!(options & A_OPT))
	{
		if((file->d_name)[0] == '.')
			return;
	}
	if(options & I_OPT)
	{
		printf("%ld ", (long int)info->st_ino);
	}
	if(options & L_OPT || options & N_OPT)
	{
		printrwx(info->st_mode);
		printf("%ld ", info->st_nlink);
		if(options & L_OPT)
			printf("%s %s ", getpwuid(info->st_uid)->pw_name,
					 getpwuid(info->st_gid)->pw_name);
		else
			printf("%ld %ld ", (long)info->st_uid,
					   (long)info->st_gid);
		printf("%ld ", (long)info->st_size);
		printdata(info);
	}
	printname(file, info);
	if(options & L_OPT || options & N_OPT)
		printf("\n");
}

void getinfo(char* path, int options)
{
	DIR* direct = opendir(path);
	printf("%s:\n", path);
	if(direct == NULL)
	{
		perror("Error with open directory");
		return;
	}
	struct dirent* file;
	struct stat info;
	while(file = readdir(direct))
	{
		char* real_path = concat(path, file->d_name);
		if(stat(real_path, &info) == -1)
		{
			perror("Status error");
			return;
		}
		printinfo(file, &info, options);
		if(options & R_OPT)
		{
			if((info.st_mode & S_IFDIR)
			    && strcmp(file->d_name, "..")
			    && strcmp(file->d_name, "."))
			{
				getinfo(real_path, options);
			}
		}
	}
	printf("\n");
	closedir(direct);
}

int get_opt(int argc, char* argv[])
{
	int flag = 0;
	for(int i = 0; i < argc; i++)
	{
		switch(getopt_long(argc, argv, LONG_OPTS, options, NULL))
		{
			case 'l':
				flag |= L_OPT;
				break;
			case 'a':
				flag |= A_OPT;
				break;
			case 'R':
				flag |= R_OPT;
			break;
			case 'i':
				flag |= I_OPT;
				break;
			case 'n':
				flag |= N_OPT;
				break;
			case -1:
				return flag;
			case '?':
				printf("Unknown option");
				return -1;
			default:
				printf("Undefined behavior");
				return -1;
		}
	}
	return -1;
}

int main(int argc, char* argv[])
{
	const int options = get_opt(argc, argv);
	if(argc == optind)
	{
		getinfo(".", options);
	}
	else
	{
		for(int i = optind; i < argc; i++)
		{
			getinfo(argv[i], options);
		}
	}
	return 0;
}
