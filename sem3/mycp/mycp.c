#include <getopt.h>
#include <errno.h>

#include "../mycat/mycat.h"

#define I_OPT 1
#define F_OPT 2
#define V_OPT 4

#define FILES_MODE 1
#define DIR_MODE 2

#define FULL_ACCESS 0777

#define LONG_OPTS "ifv"

typedef struct option OPT;

OPT options[] =
{
	{"force", 0, NULL, 'f'},
	{"interactive", 0, NULL, 'i'},
	{"verbose", 0, NULL, 'v'},
	{0, 0, 0, 0}
};

void verbose(char* path_from, char* path_to)
{
	mywrite(1, "'", sizeof("'"));
	mywrite(1, path_from, strlen(path_from));
	mywrite(1, "' -> '", sizeof("' -> '"));
	mywrite(1, path_to, strlen(path_to));
	mywrite(1, "'\n", sizeof("'\n"));
}

int get_opt(int argc, char **argv)
{
	int flag = 0;
	for(int i = 0; i < argc; i++)
	{
		switch(getopt_long(argc, argv, LONG_OPTS, options, NULL))
		{
			case 'v':
				flag |= V_OPT;
				break;
			case 'f':
				flag |= F_OPT;
				break;
			case 'i':
				flag |= I_OPT;
				break;
			case -1:
				return flag;
			case '?':
				write(2, "Unknown option", sizeof("Unknown option"));
				return -1;
			default:
				write(2, "Unknown behaviour", sizeof("Unknown behaviour"));
				return -1;
		}
	}
	return -1;
}

int dir_check(char* path)
{
	if(path[strlen(path) - 1] == '/')
		return 1;
	return 0;
}

int file_check(int argc, char **argv)
{
	int n_files = 0;
	for(int i = 1; i < argc; i++)
	{
		if(!access(argv[i], F_OK))
			n_files++;
	}
	return n_files;
}

int interactive(int options, char* path_to)
{
	int fd_test = open(path_to, O_WRONLY|O_CREAT|O_EXCL, FULL_ACCESS);
	if(errno == EEXIST)
	{
		mywrite(1, "cp: overwrite '",sizeof("cp: overwrite '"));
		mywrite(1, path_to, strlen(path_to));
		mywrite(1, "'? ", sizeof("'\n"));
		char c = getchar();
		if(c == 'Y' || c == 'y')
			return 0;
		return 1;
	}
	close(fd_test);
	return 0;
}

int file_copy(int options, char* path_from, char* path_to, int mode)
{
	if(options & I_OPT)
		if(interactive(options, path_to))
			return 0;
	int fd_from = open(path_from, O_RDONLY);
	if(fd_from < 0)
	{
		perror("File path");
		return -1;
	}
	int fd_to;
	if(mode == FILES_MODE)
		fd_to = open(path_to, O_WRONLY, FULL_ACCESS);
	if(mode == DIR_MODE)
		fd_to = open(path_to, O_WRONLY|O_CREAT|O_TRUNC, FULL_ACCESS);
	if(fd_to < 0)
	{
		perror("Where to copy error");
		return -1;
	}
	if(options & V_OPT)
		verbose(path_from, path_to);
	if(mycopy(fd_from, fd_to) < 0)
	{
		close(fd_from);
		close(fd_to);
		perror("Copy error");
		return -1;
	}
	close(fd_from);
	close(fd_to);
	return 0;
}

int dir_copy(int options, int argc, char** argv, int opt_offset)
{
	int dir_offset = strlen(argv[argc-1]);
	for (int i = opt_offset + 1; i < argc - 1; i++)
	{
		argv[argc-1] = strcat(argv[argc-1], strrchr(argv[i], '/') + 1);
		file_copy(options, argv[i], argv[argc-1], DIR_MODE);
		argv[argc-1][dir_offset] = '\0';
	}
	return 0;
}

int main(int argc, char **argv)
{
	const int options = get_opt(argc, argv);
	if(options < 0)
		return -1;
	int dir_stat = dir_check(argv[argc-1]);
	int n_files = file_check(argc, argv);
	int param_offset = (options & I_OPT)/I_OPT + (options & V_OPT)/V_OPT + (options & F_OPT)/F_OPT;
	if(dir_stat)
		dir_copy(options, argc, argv, param_offset);
	if(!dir_stat && n_files)
		file_copy(options, argv[param_offset + 1], argv[param_offset + 2], FILES_MODE);
	return 0;
}