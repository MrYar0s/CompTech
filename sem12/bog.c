#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_SIZE 4096
#define FULL_ACCESS 0777
#define TRY(FUNC)		\
	if(FUNC < 0)		\
	{			\
		perror("ERROR");\
		exit(-1);	\
	}			\

const int n_bog = 33;
int counter = 0;

struct msgbuf
{
	long mtype;
	int mtext[1];
};

typedef struct msgbuf Message;

int rcv(int key_id, Message* msg)
{
        TRY(msgrcv(key_id, msg, sizeof(*msg), 1, 0));
        return msg->mtext[0];
}

void snd(int key_id, Message* msg)
{
        TRY(msgsnd(key_id, msg, sizeof(*msg), 0));
}

void bog(int key, char* buffer, int size, int number) //number only for debug and test
{
	Message msg;
	msg.mtype = 1;
	msg.mtext[0] = counter;
	char c = 0;
	counter = rcv(key, &msg);
	int key_msg;
	while(1)
	{
		if(counter >= size)
		{
			snd(key, &msg);
			msgctl(key_msg, IPC_RMID, NULL);
			return;
		}
		char symbol = buffer[counter];
		char new_symbol;//for ignoring capital letters
		if(symbol >= 'A' && symbol <= 'Z')
		{
			new_symbol = symbol + 32;
		}
		else
		{
			new_symbol = symbol;
		}
		if(c == 0 && ((msgget(new_symbol, FULL_ACCESS | IPC_CREAT | IPC_EXCL)) >= 0))
		{
			c = new_symbol;
			printf("'%c', %d\n", symbol, number);
			counter++;
			key_msg = msgget(new_symbol, FULL_ACCESS);
		} else if(c == new_symbol)
		{
			printf("'%c', %d\n", symbol, number);
			counter++;
		} else if(c != new_symbol)
		{
			snd(key, &msg);
			counter = rcv(key, &msg);
		}
		msg.mtext[0] = counter;
	}
	snd(key, &msg);
	msgctl(key_msg, IPC_RMID, NULL);
}

void init(int key)
{
	Message msg;
	msg.mtype = 1;
	msg.mtext[0] = counter;
	snd(key, &msg);
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "Few arguments\n");
		return -1;
	}
	char buffer[MAX_SIZE];
	int size = 0;
	for(int i = 1; i < argc; i++)
	{
		int len = strlen(argv[i]);
		for(int j = 0; j < len; j++)
		{
			buffer[size] = argv[i][j];
			size++;
		}
		if(i != argc - 1)
		{
			buffer[size++] = ' ';
		}
	}
	int queue = msgget(IPC_PRIVATE, FULL_ACCESS);
	TRY(queue);
	init(queue);
	for(int i = 0; i < n_bog; i++)
	{
		pid_t bog_t = fork();
		TRY(bog_t);
		//child section
		if(bog_t == 0)
		{
			bog(queue, buffer, size, i + 1);
			return 0;
		}
	}
	int status;
	for(int i = 0; i < n_bog; i++)
	{
		TRY(wait(&status));
	}
	msgctl(queue, IPC_RMID, NULL);
	return 0;
}
