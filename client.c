#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdlib.h>
#define DATA_TYPE 255
int main()
{
	int msqid; 
	char pathname[] = "08-1a.c"; 
	key_t key; 
	int i, len; 

struct mymsgbuf
{
	long mtype;
	struct
	{
		int a;
		int b;
		pid_t pid;
	}info;
}mybuf;

struct myresmsgbuf
{
	long mtype;
	int c;
}result;

if((key = ftok(pathname,0)) < 0)
{
	printf("Can\'t generate key\n");
	exit(-1);
}


if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
{
	printf("Can\'t get msqid\n");
	exit(-1);
} 

	mybuf.mtype = DATA_TYPE;
	mybuf.info.a = rand() % 5;
	mybuf.info.b = rand() % 5;
	printf("a = %d b = %d\n", mybuf.info.a, mybuf.info.b);
	pid_t type = getpid();
	mybuf.info.pid = type; 
if (msgsnd(msqid, (struct mymsgbuf *) &mybuf, sizeof(mybuf.info), 0) < 0)
{
	printf("Can\'t send message to queue\n");
	msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
	exit(-1);
}

if(( len = msgrcv(msqid, (struct myresmsgbuf *) &result, sizeof(int), type, 0) < 0))
		{
			printf("Can\'t receive message from queue\n");
			exit(-1);
		}
		printf("answer = %d\n", result.c);

return 0;
}