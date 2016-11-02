#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>

#define TABLE_LIMIT 2
#define AMOUNT_DISHES 4

char pathname[] = "key.c"; 
int semid;

struct dishes
{
	char dish[15]; 	
	int time;	
}washer[AMOUNT_DISHES], wiper[AMOUNT_DISHES];

struct sembuf mybuf;

void sema(int number_op)
{
	mybuf.sem_op = number_op;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;
	if (semop(semid , &mybuf , 1) < 0) 
	{
		printf("Can`t wait for condition\n");
		exit(-1);
	}
}


int main()
{
	
	key_t key; 
	FILE* file;
	int i,fd[2],j,result;
	int dish_amount;
	char dish_type[15];
	char dish_type1[15];

	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) 
	{
		printf("Can`t get semid\n");
        	exit(-1);
	}

	if ((file = fopen("washer.txt","r"))==NULL) 
	{
		printf("File not found.\n");
	}

	for(i = 0; i < AMOUNT_DISHES; i++)
	{
		fscanf(file, "%s %d", washer[i].dish, &washer[i].time);
	}
	fclose(file);
	
	if ((file = fopen("wiper.txt","r"))==NULL) 
	{
		printf("File not found.\n");
	}
	for(i = 0; i < AMOUNT_DISHES; i++)
	{
		fscanf(file, "%s %d", wiper[i].dish, &wiper[i].time);
	}
	fclose(file);

	sema(2); 
	pipe(fd);
	result = fork();
	if(result > 0) 
	{
		if ((file = fopen("dishes.txt","r"))==NULL) 
		{
			printf("File not found.\n");
		}

		while(fscanf(file, "%s %d", dish_type, &dish_amount) != EOF) 
		for(i = 0; i < AMOUNT_DISHES; i++)  
			if(strcmp(dish_type, washer[i].dish) == 0) 
			{
				for(j = 0; j < dish_amount; j++)  
				{
					sleep(washer[i].time); 
					sema(-1);  
					printf("Washer wash up the %s for %d sec\n",washer[i].dish, washer[i].time);
					write(fd[1], washer[i].dish, 15); 
				}
				break;
			}
		fclose(file);
	}
	else
	{
		while(1)
		{
			read(fd[0], dish_type1, 15);
			for(i = 0; i < AMOUNT_DISHES; i++)  
			if(strcmp(dish_type1, wiper[i].dish) == 0) 
				{
					sema(1); 
					sleep(wiper[i].time); 
					printf("Wiper wipe up the %s for %d sec\n", wiper[i].dish, wiper[i].time);
				}
		}
	}
	sleep(10);
	printf("FINISH\n");
	return 0;
}
		
			
			
			
					
						
						
					
			

	
	
	
