#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#define MAX_LENGHT 100
#define send_bit SIGINT
#define read_0_bit SIGUSR1
#define read_1_bit SIGUSR2

	int bit_number = 0;
	pid_t ppid;
	char string[] = "Brother give me a bit";
	int lenght;
	char result[MAX_LENGHT];

	void readBit(int signal)
	{
		int symbol_number = bit_number / 8;
		int current_position = bit_number % 8;

		if (signal == read_1_bit)
		result[symbol_number] = result[symbol_number] | (1 << current_position);

		bit_number++;
		kill(ppid, send_bit);
	
		if ((bit_number % 8) == 0)
		{
			printf("%c\n", result[symbol_number]);
			sleep(1);
		}

	}

	void sendBit()
	{
		char bit;
		int symbol_number = bit_number / 8;
		int current_position = bit_number % 8;

		bit = string[symbol_number] & (1 << current_position);
		bit_number++;
		if(bit)
		{
			kill(ppid, read_1_bit);
		}
		else
		{
			kill(ppid, read_0_bit);
		}
		if (symbol_number == lenght)
		exit(0);
	}
int main()
{
	lenght = strlen(string);

	signal(read_0_bit, readBit);
	signal(read_1_bit, readBit);
	signal(send_bit, sendBit);

	ppid = getpid();
	pid_t pid = fork();

	if (pid != 0)
	{
		ppid = pid;
		sendBit();
	}

	while(1)
	{
	}

return 0;
}