#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main()
{	
	int i = 0, status;
	int n;
	scanf("%d", &n);
	pid_t fatherId  = getpid();

	for(i = 0; i < n; ++i)
	{	
		if (getpid() == fatherId)
		{
		printf(" i am %d , my father is %d\n ", getpid(), getppid());
		printf("\n");
		pid_t pid = fork();
		}
		if (getpid() == fatherId)
		{
		wait(&status);
		exit(0);
		}
		
		fatherId = getpid();
			
		
	}
	return 0;
}
