#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_USERS 15
#define MAX_NICK_LENTGH 40
#define MAX_LINE_LENGTH 1000
//interface
int chekUser(struct sockaddr_in* cliaddr);
void addUser(struct sockaddr_in* cliaddr, char *nickname);
void create_sockaddr(struct sockaddr_in * sockaddr, in_addr_t ip, unsigned short port);
void sendHelloMessage(struct sockaddr_in * sockaddr);
void sendAll(char *nickname, char* line);
int isPersonal(char *string);
void sendPersonalMessage(char *line, char *from);

struct user
{
    int ip;
    int port;
    char nickname[MAX_NICK_LENTGH];
    struct sockaddr_in useraddr;
} user[MAX_USERS];

int end = 0;
int clilen, n;
int sockfd;
char line[MAX_LINE_LENGTH];
char nickname[MAX_NICK_LENTGH];
char admin[] = "admin";

int main()
{
    struct sockaddr_in servaddr, cliaddr;
    create_sockaddr(&servaddr, htonl(INADDR_ANY), htons(51000));

    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror(NULL);
        exit(1);
    } else 
    {
        printf("Sock fd:%d\n", sockfd);
    } 
    
    if (bind(sockfd, (struct sockaddr*) &servaddr, 
        sizeof(servaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
  
    while (1)
    {
        clilen = sizeof(cliaddr);
        if ((n = recvfrom(sockfd, nickname, 999, 0, 
            (struct sockaddr*)&cliaddr, (unsigned int *)&clilen)) < 0)
        {
            perror(NULL);
            close(sockfd); 
            exit(1);
        }
        if ((n = recvfrom(sockfd, line, 999, 0, 
            (struct sockaddr*)&cliaddr, (unsigned int *)&clilen)) < 0)
        {
            perror(NULL);
            close(sockfd); 
            exit(1);
        }
        if (chekUser(&cliaddr)) {
            addUser(&cliaddr, nickname);
            sendHelloMessage(&cliaddr);
        }

        printf("%s -> %s\n", nickname, line);

        if (isPersonal(line))
        {
            printf("personal message from %s\n", nickname);
            sendPersonalMessage(line, nickname);
        }
        else{
            sendAll(nickname, line);
        }
        
        
    }
    return 0;
}
void sendAll(char *nickname, char* line) {
    for (int i = 0; i < end; i++)
    {
        if (sendto(sockfd, nickname, strlen(nickname), 0,
            (struct sockaddr*)&(user[i].useraddr), clilen) < 0)
        {
            perror(NULL);
            close(sockfd); 
            exit(1);
        }
        if (sendto(sockfd, line, strlen(line), 0,
            (struct sockaddr*)&(user[i].useraddr), clilen) < 0)
        {
            perror(NULL);
            close(sockfd); 
            exit(1);
        }
    }
}

int chekUser(struct sockaddr_in* cliaddr) {
    for (int i = 0; i < end; i++)
    {
        if ((cliaddr->sin_port == user[i].port) && (cliaddr->sin_addr.s_addr == user[i].ip)){
            return 0;
        }
    }
    return 1;
}
void addUser(struct sockaddr_in* cliaddr, char *nick) {
    user[end].ip = cliaddr->sin_addr.s_addr;
    user[end].port = cliaddr->sin_port;
    strcpy(user[end].nickname, nick);
    create_sockaddr(&user[end].useraddr, user[end].ip, user[end].port);
    end++;
}

void create_sockaddr(struct sockaddr_in * sockaddr, in_addr_t ip, unsigned short port) {
    bzero(sockaddr, sizeof(sockaddr));
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = port;
    sockaddr->sin_addr.s_addr = ip;
}
void sendHelloMessage(struct sockaddr_in * sockaddr) {
    
    char welcome[] = "Welcome to the chat!\nUse @nickname before your message\nto send personal message\n\n";
    if (sendto(sockfd, admin, strlen(admin), 0,
            (struct sockaddr*)sockaddr, clilen) < 0)
        {
            perror(NULL);
            close(sockfd); 
            exit(1);
        }
    if (sendto(sockfd, welcome, strlen(welcome), 0,
       (struct sockaddr*)sockaddr, clilen) < 0)
    {
        perror(NULL);
        close(sockfd); 
        exit(1);
    }
}
int isPersonal(char *string) {
    if (strchr(string, '@') == NULL){
        return 0;
    }
    else return 1;
}
void sendPersonalMessage(char *line, char *from) {
    char *spaceSymbol = strchr(line, ' ');
    int nickLentgh = spaceSymbol - line - 1;
    char reciever[MAX_NICK_LENTGH];
    strncpy(reciever,line + 1, nickLentgh);
    reciever[nickLentgh] = '\0';

    for (int i = 0; i < end; i++) {
        if (strcmp(reciever, user[i].nickname) == 0)
        {
            if (sendto(sockfd, from, strlen(from), 0,
            (struct sockaddr*)(&user[i].useraddr), clilen) < 0)
            {
                perror(NULL);
                close(sockfd); 
                exit(1);
            }
            if (sendto(sockfd, spaceSymbol + 1, strlen(spaceSymbol + 1), 0,  //spaceSymbol is a pointer to string without nick
            (struct sockaddr*)(&user[i].useraddr), clilen) < 0)
            {
                perror(NULL);
                close(sockfd); 
                exit(1);
            }
            return;
        }
    }
    for (int i = 0; i < end; i++) {
        if (strcmp(from, user[i].nickname) == 0){
            
            char noExist[] = "User with this nickname is offline now\nTry to write him later\n";
            
            if (sendto(sockfd, admin, strlen(admin), 0,
            (struct sockaddr*)(&user[i].useraddr), clilen) < 0)
            {
                perror(NULL);
                close(sockfd); 
                exit(1);
            }
            if (sendto(sockfd, noExist, strlen(noExist), 0,  
            (struct sockaddr*)(&user[i].useraddr), clilen) < 0)
            {
                perror(NULL);
                close(sockfd); 
                exit(1);
            }
        }
    }
}