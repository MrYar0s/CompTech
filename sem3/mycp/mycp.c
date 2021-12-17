    //////////////////////////////////
   //            MY CP             //
  //          VERSION 1.0         //
 //         Made By MrYar0s      //
//////////////////////////////////
#include <getopt.h>
#include <errno.h>
#include <dirent.h>

//at this .h file defined functions
//like mywrite and myread;
//included other .h files
#include "../mycat/mycat.h"

//params for flag
//0001
#define I_OPT 1
//0010
#define F_OPT 2
//0100
#define V_OPT 4

//define of full access
#define FULL_ACCESS 0777

#define LONG_OPTS "ifv"

typedef struct option OPT;

//Array of possible options
OPT options[] =
{
	{"force", 0, NULL, 'f'},
	{"interactive", 0, NULL, 'i'},
	{"verbose", 0, NULL, 'v'},
	{0, 0, 0, 0}
};

//Function to realize -v param
//Just print file destination
void verbose(char* path_from, char* path_to)
{
	mywrite(1, "'", sizeof("'"));
	mywrite(1, path_from, strlen(path_from));
	mywrite(1, "' -> '", sizeof("' -> '"));
	mywrite(1, path_to, strlen(path_to));
	mywrite(1, "'\n", sizeof("'\n"));
}

//Function to realize -i param
//Ask you about do you want to rewrite file
int interactive(char* path_to)
{
	open(path_to, O_WRONLY|O_CREAT|O_EXCL, FULL_ACCESS);
	if(errno == EEXIST)
	{
		mywrite(1, "cp: overwrite '",sizeof("cp: overwrite '"));
		mywrite(1, path_to, strlen(path_to));
		mywrite(1, "'? ", sizeof("'? "));
		char c = 0;
		while((c = getchar()) == '\n')
			fflush(stdin);
		if(c == 'Y' || c == 'y')
		{
			return 0;
		}
		return 1;
	}
	return 0;
}

//Function to realize -f param
//Try to write in file, when you
//can not interact with it
int force(char* path_to)
{
	if(open(path_to, O_WRONLY|O_CREAT|O_EXCL, FULL_ACCESS) < 0)
		if(errno == EEXIST)
		{
			if(open(path_to, O_WRONLY, FULL_ACCESS) < 0)
			{
				if(remove(path_to) < 0)
				{
					perror("Unsuccessful remove");
					return -1;
				}
				if(open(path_to, O_WRONLY|O_CREAT, FULL_ACCESS) < 0)
				{
					perror("Fail with force");
					return -1;
				}
			}
			return 0;
		}
	return 0;
}

//Function to check options and set flag
//We check argv with n = argc params
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

//Function that check
//is "path" a directory
int dir_check(char* path)
{
	DIR* dir = opendir(path);
	if(dir)
	{
		closedir(dir);
		return 1;
	} else if(errno == ENOENT)
	{
		return 0;
	}
	return 0;
}

//Function that count number of files
//from argv with n = argc params
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

//Function that copy file from "path_from" to "path_to"
//"path_to" can be only a file
//"options" is a flag of possible options
int file_copy(int options, char* path_from, char* path_to)
{
	int fd_from = open(path_from, O_RDONLY);
	if(fd_from < 0)
	{
		perror("File path");
		return -1;
	}
	if(options & I_OPT)
	{
		if(interactive(path_to))
			return -1;
	}
	if(options & F_OPT)
	{
		if(force(path_to))
			return -1;
	}
	int fd_to = open(path_to, O_WRONLY|O_CREAT|O_TRUNC, FULL_ACCESS);
	if(fd_to < 0)
	{
		perror("Where to copy error");
		return -1;
	}
	if(options & V_OPT)
		verbose(path_from, path_to);
	if(mycopy(fd_from, fd_to) < 0)
	{
		perror("Copy error");
		return -1;
	}
	close(fd_from);
	close(fd_to);
	return 0;
}

//Function that get file name from "file_path"
//If we have file path like "file1"
//it will return "file_path" away
//If we have file path like "./file1" or "/file1"
//it will return "file_name" that will be "file"
char* get_filename(char* file_path)
{
	char* file_name = strrchr(file_path, '/');
	if(file_name == NULL)
		return file_path;
	return file_name;
}

//Function that makes path for new file
//f.e. "dir_name" = test
//f.e. "file" = file1
//f.e. we will get "dir_name" = test/file1
//f.e. "dir_name" = test/
//f.e. "file" = file1
//f.e. "dir_name" = test/file1
char* concat(char* dir_name, char* file_name)
{
	if(dir_name[strlen(dir_name) - 1] != '/')
	{
		dir_name = strcat(dir_name, "/");
	}
	dir_name = strcat(dir_name, file_name);
	return dir_name;
}

//Function that calls when we need to copy N files to a directory
//It calls to cat dest name and file name
int dir_copy(int options, int argc, char** argv)
{
	int dir_offset = strlen(argv[argc-1]);
	char* dir_name = argv[argc-1];
	for (int i = optind; i < argc - 1; i++)
	{
		dir_name = concat(dir_name, get_filename(argv[i]));
		file_copy(options, argv[i], dir_name);
		dir_name[dir_offset] = '\0';
	}
	return 0;
}

int main(int argc, char **argv)
{
	//we get number of options in "options"
	const int options = get_opt(argc, argv);
	if(options < 0)
		return -1;
	//check argv[argc-1] is dir or not
	int dir_stat = dir_check(argv[argc-1]);
	//count number of files
	int n_files = file_check(argc, argv);
	//choose mode that we will use
	if(dir_stat)//copy to dir mode
	{
		dir_copy(options, argc, argv);
	} else if(!dir_stat && n_files)//copy file2file
	{
		file_copy(options, argv[optind], argv[optind + 1]);
	}
	else//didn't find dir or file
	{
		perror("No such mode");
		return -1;
	}
	return 0;
}
