/* Пример простого TCP-сервера для сервиса echo */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
void* op(void *arg);
pthread_t threads[15];
int usersfd[15];

int end = 0;

int sockfd, newsockfd; /* Дескрипторы для 
слушающего и присоединенного сокетов */
int clilen; /* Длина адреса клиента */
int n; /* Количество принятых символов */
char line[1000]; /* Буфер для приема информации */
struct sockaddr_in servaddr, cliaddr;

int main()
{
     /* Структуры 
        для размещения полных адресов сервера и 
        клиента */
    /* Создаем TCP-сокет */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }
    /* Заполняем структуру для адреса сервера: семейство
    протоколов TCP/IP, сетевой интерфейс – любой, номер 
    порта 51000. Поскольку в структуре содержится 
    дополнительное не нужное нам поле, которое должно 
    быть нулевым, побнуляем ее всю перед заполнением */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family= AF_INET;
    servaddr.sin_port= htons(50007);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* Настраиваем адрес сокета */
    if(bind(sockfd, (struct sockaddr *) &servaddr, 
    sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    /* Переводим созданный сокет в пассивное (слушающее) 
    состояние. Глубину очереди для установленных 
    соединений описываем значением 5 */
    if(listen(sockfd, 5) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    /* Основной цикл сервера */
    while(1){
        /* В переменную clilen заносим максимальную
        длину ожидаемого адреса клиента */
        clilen = sizeof(cliaddr);
        /* Ожидаем полностью установленного соединения
        на слушающем сокете. При нормальном завершении 
        у нас в структуре cliaddr будет лежать полный 
        адрес клиента, установившего соединение, а в 
        переменной clilen – его фактическая длина. Вызов
        же вернет дескриптор присоединенного сокета, через
        который будет происходить общение с клиентом. 
        Заметим, что информация о клиенте у нас в
        дальнейшем никак не используется, поэтому 
        вместо второго и третьего параметров можно 
        было поставить значения NULL. */
        if((newsockfd = accept(sockfd, 
        (struct sockaddr *) &cliaddr, (unsigned int*)&clilen)) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        usersfd[end] = newsockfd;
        pthread_create(&(threads[end]) , 
                    (pthread_attr_t*)NULL, 
                    op,
                    (void*)&(usersfd[end]));
        
        end++; 
    }
        
}

void* op(void *arg){
    int *fd = (int *)arg;
    printf("NEW USER END = %d FD = %d\n", end,newsockfd);

    while(1) {
        n = read(*fd, line, 999);
        printf("СООБЩЕНИЕ ОТ FD = %d\n", *fd);
        for (int i = 0; i < end; i++){
            if (usersfd[i] == *fd) continue;
            if((n = write(usersfd[i], line, 
            strlen(line)+1)) < 0){
                perror(NULL);
                close(sockfd);
                close(*fd);
                exit(1);
            }
        }
    }

    close(*fd);
    return NULL;

}
    
