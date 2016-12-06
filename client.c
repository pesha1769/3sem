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

int main(int argc, char* argv[]) 
{
    int sockfd;
    int servlen, len;
    struct sockaddr_in servaddr, cliaddr;
    pid_t pid;
    char message[MAX_MESSAGE_SIZE];
    
    if (argc != 3) {
        printf("Usage: ./a.out <IP address> <Nickname>\n");
        exit(1);
    }
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror(NULL);
        exit(1);
    }
      
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
      
    if (bind(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0) {
        printf("Invalid IP address\n");
        close(sockfd);
        exit(1);
    }

    if ((pid = fork()) == 0) 
    {
        while (1) 
        {
            servlen = sizeof(servaddr);
            if (recvfrom(sockfd, message, MAX_MESSAGE_SIZE, 0, (struct sockaddr*)&servaddr, &servlen) < 0)
            {
                perror(NULL);
                close(sockfd);
                exit(1);
            }
            printf("%s", message);
        }
    }else 
    {
        if (sendto(sockfd, argv[2], 20, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        while (1) 
        {
            fgets(message, MAX_MESSAGE_SIZE, stdin);
            if (sendto(sockfd, message, MAX_MESSAGE_SIZE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
            {
                perror(NULL);
                close(sockfd);
                exit(1);
            }
        }
    }
    close(sockfd);
    return 0;
}
