#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#define number 100
int main()
{

int *array; 
int shmid; 
int new = 1;
char pathname[] = "06-1a.c"; 
key_t key; 
if((key = ftok(pathname,0)) < 0)
{
	printf("Can\'t generate key\n");
	exit(-1);
}


if((shmid = shmget(key, number*sizeof(char), 0666|IPC_CREAT|IPC_EXCL)) < 0)
{
	if(errno != EEXIST)
	{
		printf("Can\'t create shared memory\n");
		exit(-1);
	}
	 else 
	{
		if((shmid = shmget(key, number*sizeof(char), 0)) < 0)
		{
			printf("Can\'t find shared memory\n");
			exit(-1);
		}
		new = 0;
	}
}

if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1))
{
	printf("Can't attach shared memory\n");
	exit(-1);
}

int fd;
size_t bytesRead;
char* word = malloc (number* sizeof(char));
fd = open("txt.txt", O_RDONLY, 1);

int len = 0;
while ((bytesRead = read(fd, word, number)) > 0 )
{
	memcpy(array + len, word, bytesRead);
	len += bytesRead;	
}
if(shmdt(array) < 0)
{
	printf("Can't detach shared memory\n");
	exit(-1);
}
return 0;
}

