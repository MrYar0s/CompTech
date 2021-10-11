#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SIZE 4096

void prompt()
{
	printf("\033[0;32m");
	printf("ready for execution$ ");
	printf("\033[0m");
}

char* readline()
{
	char* line = NULL;
	size_t len = 0;
	if(getline(&line, &len, stdin) < 0)
	{
		if(feof(stdin))
		{
			exit(0);
		}
		perror("Getline error");
		exit(1);
	}
	for (size_t i = 0; i < len; ++i)
	{
		if(line[i] == '\n')
		{
			line[i] = '\0';
		}
	}
	if(!strcmp(line, "exit"))
	{
		exit(0);
	}
	return line;
}

size_t cmdcount(char* line)
{
	char* start;
	size_t n_cmd = 1;
	while(start = strrchr(line, '|'))
	{
		line[(start-line)] = ',';
		n_cmd++;
	}
	return n_cmd;
}

char** cmdparse(char* line, size_t n_cmd)
{
	char** cmd = (char**)malloc(n_cmd*sizeof(char*));
	char* mod_line = (char*)malloc(strlen(line) + 1);
	strcpy(mod_line, line);
	char* start = mod_line;
	for (size_t i = 0; i < n_cmd; ++i)
	{
		char* end = strchr(mod_line, ',');
		if(i != n_cmd - 1)
		{
			mod_line[(end - mod_line)] = '\0';
		}
		if(i)
		{
			start = strrchr(mod_line, '|') + 1;
		}
		cmd[i] = (char*)malloc(strlen(start) + 1);
		strcpy(cmd[i], start);
		if(i != n_cmd - 1)
		{
			mod_line[(end - mod_line)] = '|';
		}
	}
	return cmd;
}

char** splitcmd(char* cmd)
{
	char** splitted = (char**)malloc(MAX_SIZE);
	char* del = strtok(cmd, " ");
	size_t i = 0;
	while(del)
	{
		splitted[i++] = (char*)malloc(MAX_SIZE);
		strcpy(splitted[i-1], del);
		del = strtok(NULL, " ");
	}
	return splitted;
}

int main()
{
	while(1)
	{
		prompt();
		char* line = readline();

		size_t n_cmd = cmdcount(line);
		char** cmd = cmdparse(line, n_cmd);

		int fd_in = 0;

		for (size_t i = 0; i < n_cmd; ++i)
		{
			int fd[2];
			pipe(fd);

			int status;
			pid_t id = fork();

			if(id == 0)
			{
				char** splitted = splitcmd(cmd[i]);
				dup2(fd_in, 0);
				if(i != n_cmd - 1)
				{
					dup2(fd[1], 1);
				}
				close(fd[0]);
				execvp(splitted[0], splitted);
				free(splitted);
				perror("Error with exec");
				return -1;
			} else if(id > 0)
			{
				if(wait(&status) < 0)
				{
					perror("Wait error");
					return -1;
				}
				fd_in = fd[0];
				close(fd[1]);
			} else
			{
				perror("Bad fork");
				return -1;
			}
		}
		free(cmd);
		free(line);
	}
	return 0;
}