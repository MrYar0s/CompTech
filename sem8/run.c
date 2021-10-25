#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

#define FULL_ACCESS 0777

#define RUNNER_CAME (n+2)
#define RUN_END (n+3)

#define TRY(a) ({if(a < 0){perror("Error"); exit(EXIT_FAILURE);}})

#define USEC 1000000

double print_time(struct timeval t1, struct timeval t2)
{
	double usec = ((double)(t2.tv_usec - t1.tv_usec))/USEC;
	double sec = (double)(t2.tv_sec - t1.tv_sec);
	return usec+sec;
}

struct msgbuf
{
	long type;
};

typedef struct msgbuf Message;

void rcv(int key_id, int data, Message msg)
{
	if(msgrcv(key_id, &msg, sizeof(msg), data, 0) < 0)
	{
		perror("Msgrcv");
		exit(EXIT_FAILURE);
	}
}

void snd(int key_id, Message msg)
{
	if(msgsnd(key_id, &msg, sizeof(msg), 0) < 0)
	{
		perror("Msgsnd");
		exit(EXIT_FAILURE);
	}
}

void judge(int n, int key_id)
{
	Message msg;
	int judge_id = n + 1;

	//wait for runners
	printf("Judje here\n");
	for(int i = 1; i <= n; i++)
	{
		rcv(key_id, RUNNER_CAME, msg);
	}

	//starts of run
	struct timeval t1 = {0, 0};
	struct timeval t2 = {0, 0};

	printf("Run started\n");
	msg.type = 1;
	snd(key_id, msg);

	TRY(gettimeofday(&t1, NULL));

	//wait for end of the run
	printf("Waiting for the end of the run\n");
	rcv(key_id, judge_id, msg);

	//stop of run
	TRY(gettimeofday(&t2, NULL));
	printf("The run ended in %.3lf\n", print_time(t1,t2));
	for(int i = 1; i <= n; i++)
	{
		msg.type = RUN_END;
		snd(key_id, msg);
	}
	printf("Judje is going home\n");
}

void runner(int n, int run_id, int key_id)
{
	Message msg;

	//come to run
	msg.type = RUNNER_CAME;
	printf("Runner %d came\n", run_id);
	snd(key_id, msg);

	//start of run
	rcv(key_id, run_id, msg);
	
	printf("Runner %d started\n", run_id);
	
	msg.type = run_id + 1;
	snd(key_id, msg);
	printf("Runner %d ended\n", run_id);
	
	//stop run
	rcv(key_id, RUN_END, msg);
	printf("Runner %d is going home\n", run_id);
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Give me number of runners\n");
		return -1;
	}
	int num_of_runners = atoi(argv[1]);
	if(num_of_runners < 1)
	{
		printf("Half of runner doesn't exist\n");
		return -1;
	}
	int status;
	int key = msgget(IPC_PRIVATE, FULL_ACCESS);
	TRY(key);

	//judje
	pid_t judge_id = fork();
	TRY(judge_id);
	if(judge_id == 0)
	{
		judge(num_of_runners, key);
		return 0;
	}

	//runners
	for(int i = 1; i <= num_of_runners; i++)
	{
		pid_t pid = fork();
		TRY(pid);
		if(pid == 0)
		{
			runner(num_of_runners, i, key);
			return 0;
		}
	}
	for(int i = 0; i < num_of_runners + 1; i++)
	{
		wait(&status);
	}
	msgctl(key, IPC_RMID, NULL);
}
