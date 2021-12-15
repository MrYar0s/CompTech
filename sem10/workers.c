#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ERROR -1
#define FULL_ACCESS 0777
#define TRY(a) ({if(a < 0){perror("Error"); exit(ERROR);}})
#define TISKI 0

struct msgbuf
{
	long type;
	long text;
};

typedef struct msgbuf Message;

int rcv(int key_id, Message* msg)
{
	TRY(msgrcv(key_id, msg, sizeof(*msg), 1, 0));
	return msg->text;
}

void snd(int key_id, Message* msg)
{
	TRY(msgsnd(key_id, msg, sizeof(*msg), 0));
}

void take(int q_vint, int q_bolt, int n_vint, int n_bolt)
{
	Message msg = {1, 0};
	for(int i = 0; i < n_vint; i++)
	{
		snd(q_vint, &msg);
	}
	for(int i = 0; i < n_bolt; i++)
	{
		snd(q_bolt, &msg);
	}
}

void vint(int q_vint, int q_bolt, int q_tiski, int n_vint, int n_bolt, int n_details)
{
	for(int i = 0; i < n_details; i++)
	{
		Message msg = {1, 0};
		rcv(q_vint, &msg);
		printf("Vintovik ready\n");
		int result = rcv(q_tiski, &msg);
		result++;
		printf("Vstavil vint, otoshel %d\n", result);
		msg.text = result;
		if(result == n_vint+n_bolt)
		{
			take(q_vint, q_bolt, n_vint, n_bolt);
			printf("Detaile was produced\n");
			msg.text = 0;
		}
		snd(q_tiski, &msg);
	}
}

void bolt(int q_vint, int q_bolt, int q_tiski, int n_vint, int n_bolt, int n_details)
{
	for(int i = 0; i < n_details; i++)
	{
		Message msg = {1, 0};
		rcv(q_bolt, &msg);
		printf("Boltovik ready\n");
		int result = rcv(q_tiski, &msg);
		result++;
		printf("Vstavil bolt, otoshel %d\n", result);
		msg.text = result;
		if(result == n_vint+n_bolt)
		{
			take(q_vint, q_bolt, n_vint, n_bolt);
			printf("Detaile was produced\n");
			msg.text = 0;
		}
		snd(q_tiski, &msg);
	}
}

int main(int argc, char** argv)
{
	if(argc < 4)
	{
		printf("Give me number of vint and bolt\n");
		return -1;
	}
	int num_of_vint = atoi(argv[1]);
	int num_of_bolt = atoi(argv[2]);
	int num_of_details = atoi(argv[3]);

	int q_bolt = msgget(IPC_PRIVATE, FULL_ACCESS);
	int q_vint = msgget(IPC_PRIVATE, FULL_ACCESS);
	int q_tiski = msgget(IPC_PRIVATE, FULL_ACCESS);

	int status;
	Message msg = {1, 0};
	snd(q_tiski, &msg);
	take(q_vint, q_bolt, num_of_vint, num_of_bolt);
	for(int i = 0; i < num_of_vint; i++)
	{
		pid_t vintovik = fork();
		TRY(vintovik);
		if(vintovik == 0)
		{
			vint(q_vint, q_bolt, q_tiski, num_of_vint, num_of_bolt, num_of_details);
			return 0;
		}
	}
	for(int i = 0; i < num_of_bolt; i++)
	{
		pid_t boltovik = fork();
		TRY(boltovik);
		if(boltovik == 0)
		{
			bolt(q_vint, q_bolt, q_tiski, num_of_vint, num_of_bolt, num_of_details);
			return 0;
		}
	}
	for(int i = 0; i < num_of_bolt + num_of_vint; i++)
	{
		wait(&status);
	}
	msgctl(q_vint, IPC_RMID, NULL);
	msgctl(q_bolt, IPC_RMID, NULL);
	msgctl(q_tiski, IPC_RMID, NULL);
}
