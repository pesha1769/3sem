#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#define STRING "hello!"
struct sembuf mybuf;
int semid;
void sema(int param)
{
	mybuf.sem_op = param;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;
	if(semop(semid, &mybuf, 1) < 0)
	{	
		printf("Can`t wait for condition\n");
		exit(-1);
	}
}

int main()
{
	char pathname[] = "08-1a.c"; 
	key_t key; 
	int i = 0;
	FILE* fd = NULL;
	fd = fopen("txt.txt", "a");
	if (fd == NULL) 
	{
		perror("txt.txt");
		return 7;
	}

if((key = ftok(pathname, 0)) < 0)
{	
	printf("Can`t generate key\n");
	exit(-1);
}

if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
{
	printf("Can`t get semid\n");
	exit(-1);
} 

sema(1);
while(1)
{ 
	sema(-1);
	fprintf(fd, STRING);
	sema(1);
}
	fclose(fd);

return 0;
}
