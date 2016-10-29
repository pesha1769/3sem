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
#define MAX_THREADS 2

int msqid;
int semid;

struct sembuf semobuf;
void sema(int a)
{
	semobuf.sem_num = 0;
	semobuf.sem_op = a;
	semobuf.sem_flg = 0;
	if (semop(semid , &semobuf , 1) < 0) 
	{
		printf("Can`t wait for condition\n");
		exit(-1);
	}
}

struct mymsgbuf
{
	long mtype;
	struct
	{
		int a;
		int b;
		pid_t pid;
	}info;
};

struct myresmsgbuf
{
	long mtype;
	int c;
};


void* my_thread(void* dummy)
{
	struct mymsgbuf buf = *(struct mymsgbuf*)(dummy);
	int a = buf.info.a;
	int b = buf.info.b;
	printf("In thread a  = %d b = %d\n", a, b);
	printf("Calculating...\n");
	sleep(2);
	struct myresmsgbuf res;
	res.mtype = buf.info.pid;
	res.c = a + b;
	if (msgsnd(msqid, (struct myresmsgbuf *) &res, sizeof(res.c), 0) < 0)
	{
		printf("Can\'t send message to queue\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
		exit(-1);
	}
	sema(1);
	return NULL;
}




int main() 
{
	pthread_t thread_id;
	char pathname[] = "08-1a.c"; 
	key_t key; 
	int i, len; 



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

	if ((semid = semget(key, 1, 0666|IPC_CREAT)) < 0) 
	{
		printf("Can't create semaphore set\n");
		exit(-1);
	}
	sema(MAX_THREADS);

	struct mymsgbuf mybuf;

	while(1)
	{
		if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf.info), DATA_TYPE, 0) < 0))
		{
			printf("Can\'t receive message from queue\n");
			exit(-1);
		}
		int result = pthread_create(&thread_id, (pthread_attr_t*)NULL, my_thread, &mybuf);
	}

	return 0;
}