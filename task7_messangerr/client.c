#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1000


void create_sockaddr(struct sockaddr_in * sockaddr, in_addr_t ip, unsigned short port);
void sendMessage(char *message, char *from, struct sockaddr_in serverSock);
void recieveMessage();
void joinToChat(char * nickname, struct sockaddr_in serverSock);

int sockfd;
char sendLine[MAX_LINE_LENGTH], recvLine[MAX_LINE_LENGTH];
struct sockaddr_in servaddr, cliaddr;
char nickname[50];
int n, len;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("Usage: ./a.out <IP address> <Nickname>\n");
        exit(1);
    }
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror(NULL);
        exit(1);
    }
  
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
    if (bind(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
    {
        printf("Invalid IP address\n");
        close(sockfd);
        exit(1);
    }
    joinToChat(argv[2], servaddr);
    pid_t pid = fork();
    if (pid == 0) {
        //procces to recieve messages from server
        while(1) {
            recieveMessage();
            /*
            *   Server send all messages for all users
            *   this if-block ignores own messages 
            */
            if ((strcmp(argv[2], nickname) )) {  
                printf("%s : ", nickname); 
                printf("%s", recvLine);
            }
        }
    }
    else {
        //procces to send message to server
        while(1){
            fgets(sendLine, MAX_LINE_LENGTH, stdin);
            sendMessage(sendLine, argv[2], servaddr);
        }
    }
    return 0;
}

void recieveMessage() {
    
    if ((n = recvfrom(sockfd, nickname, MAX_LINE_LENGTH - 1, 0, (struct sockaddr*) NULL, NULL)) < 0)
            {
                perror(NULL);
                close(sockfd);
                exit(1);
            }
            nickname[n] = '\0';
            if ((n = recvfrom(sockfd, recvLine, MAX_LINE_LENGTH - 1, 0, (struct sockaddr*) NULL, NULL)) < 0)
            {
                perror(NULL);
                close(sockfd);
                exit(1);
            }
            recvLine[n] = '\0';
}
void sendMessage(char *message, char *from, struct sockaddr_in serverSock) {
    
    if (sendto(sockfd, from, strlen(from) + 1, 0, (struct sockaddr*)&serverSock, sizeof(serverSock)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    
    if (sendto(sockfd, message, strlen(message) + 1, 0, (struct sockaddr*)&serverSock, sizeof(serverSock)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    } 
}
void create_sockaddr(struct sockaddr_in * sockaddr, in_addr_t ip, unsigned short port) {
    bzero(sockaddr, sizeof(sockaddr));
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = port;
    sockaddr->sin_addr.s_addr = ip;
}

void joinToChat(char * nickname, struct sockaddr_in serverSock){
    if (sendto(sockfd, nickname, strlen(nickname) + 1, 0, (struct sockaddr*)&serverSock, sizeof(serverSock)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    char hello[] = "Hello Everybody!\n";
    if (sendto(sockfd, hello, strlen(hello) + 1, 0, (struct sockaddr*)&serverSock, sizeof(serverSock)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

}