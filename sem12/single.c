#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define FULL_ACCESS 0777
#define IDENTIFIER 924551

#define TRY(FUNC)               \
        if(FUNC < 0)            \
        {                       \
                perror("ERROR");\
                exit(-1);       \
        }			\

#define PRINT(a)		\
	write(1, a, 1)		\

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

void print_hello()
{
	PRINT("H");
	sleep(1);
	PRINT("E");
	sleep(1);
	PRINT("L");
        sleep(1);
	PRINT("L");
        sleep(1);
	PRINT("O");
        sleep(1);
	PRINT(",");
        sleep(1);
	PRINT(" ");
        sleep(1);
	PRINT("W");
        sleep(1);
	PRINT("O");
        sleep(1);
	PRINT("R");
        sleep(1);
	PRINT("L");
        sleep(1);
	PRINT("D");
        sleep(1);
	PRINT("!");
        sleep(1);
}

void print_goodbye()
{
	PRINT("G");
        sleep(1);
	PRINT("O");
        sleep(1);
	PRINT("O");
        sleep(1);
	PRINT("D");
        sleep(1);
	PRINT("B");
        sleep(1);
	PRINT("Y");
        sleep(1);
	PRINT("E");
        sleep(1);
	PRINT(",");
        sleep(1);
	PRINT(" ");
        sleep(1);
	PRINT("W");
        sleep(1);
	PRINT("O");
        sleep(1);
	PRINT("R");
        sleep(1);
	PRINT("L");
        sleep(1);
	PRINT("D");
        sleep(1);
	PRINT("!");
        sleep(1);
}

int main()
{
	Message msg;
	msg.mtype = 1;
	int status = 0;
	int key = msgget(IDENTIFIER, FULL_ACCESS | IPC_CREAT | IPC_EXCL);
	if(key < 0)
	{
		key = msgget(IDENTIFIER, FULL_ACCESS);
		status = rcv(key, &msg);
	}
	msg.mtext[0] = status + 1;
	snd(key, &msg);
	if(msg.mtext[0] == 1)
	{
		print_hello();
	}
	else
	{
		print_goodbye();
	}
	status = rcv(key, &msg);
	msg.mtext[0] = status - 1;
	snd(key, &msg);
}
