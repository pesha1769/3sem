#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_MESSAGE_SIZE 1000

struct User {
    struct sockaddr_in cliaddr;
    char nick[20];
};

void Split(char* string, char* delimiters, char*** tokens, int* tokensCount)
{
    char* temp = NULL;
    temp = strtok(string, delimiters);
    if (temp == NULL) 
    {
        printf("error");
        return;
    }
    while (temp != NULL)
    {
        strcpy((*tokens)[*tokensCount], temp);
        (*tokensCount)++;
        temp = strtok(NULL, delimiters);
    }
}


int main() 
{
    int sockfd;
    int clilen;
    char line[MAX_MESSAGE_SIZE], subline[MAX_MESSAGE_SIZE];
    char message[MAX_MESSAGE_SIZE];
    struct sockaddr_in servaddr, cliaddr;
    struct User uTable[15];
    int i, n = 0, k;
    char* delimiters = (char*) malloc (100 * sizeof(char));
    char** tokens = (char**) malloc (sizeof(char*) * 100);
    int m = 0;
    for(m = 0; m < 100; m++)
		tokens[m] = (char*)malloc(100 * sizeof(char));
	int tokensCount = 0;
  
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror(NULL);
        exit(1);
    } 
  
    if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
  
    while (1) 
    {
        clilen = sizeof(cliaddr);
        if (recvfrom(sockfd, line, MAX_MESSAGE_SIZE, 0, (struct sockaddr*)&cliaddr, &clilen) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(1);
        }

        for (i = 0; i < n; i++)
            if (cliaddr.sin_addr.s_addr == uTable[i].cliaddr.sin_addr.s_addr && cliaddr.sin_port == uTable[i].cliaddr.sin_port)
            {
                Split(line, "@", &tokens, &tokensCount);
                strcpy(message, uTable[i].nick);
                strcat(message, "->");
                strcat(message, tokens[tokensCount - 1]);
                k = i;
                printf("%s", message);
                break;
            }

        if (n == i) 
        {
            bzero(&uTable[n].cliaddr, sizeof(uTable[n].cliaddr));
            uTable[n].cliaddr.sin_family = AF_INET;
            uTable[n].cliaddr.sin_addr.s_addr = cliaddr.sin_addr.s_addr;
            uTable[n].cliaddr.sin_port = cliaddr.sin_port;
            strcpy(uTable[n].nick, line);
            n++;

        } else 
        {
        	
            for (i = 0; i < n; i++)
            	if (i != k)
            		{
                    	clilen = sizeof(uTable[i].cliaddr);
                    	if (tokensCount > 1)
                    		{	
                    			int j = 0;
                    			for (j = 0; j < tokensCount - 1; ++j)
                    			{
                    				if(strcmp(tokens[j], uTable[i].nick) == 0)
                    					sendto(sockfd, message, MAX_MESSAGE_SIZE, 0, (struct sockaddr*)&uTable[i].cliaddr, clilen);
                        									
                    			}

                    		}else
                   				{    
                       				 if (sendto(sockfd, message, MAX_MESSAGE_SIZE, 0, (struct sockaddr*)&uTable[i].cliaddr, clilen) < 0)
                        				{
                            				perror(NULL);
                   							close(sockfd);
                       						exit(1);
                    					}
                				}
                	}
        }
    }
    return 0;
}
