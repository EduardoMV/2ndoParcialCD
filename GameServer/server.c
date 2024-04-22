
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <unistd.h> 
#include <stdlib.h>
#include "blackjack.h"

#define PORT 8080 
#define MAXLINE 1000 

int listenfd;
int fd;
struct card deck[52];


void aborta_handler(int sig)
{
	printf("....abortando el proceso servidor %d\n", sig);
    close(listenfd);
    close(fd);
	exit(1);
} 

int main() 
{   
    if (signal(SIGINT, aborta_handler) == SIG_ERR)
	{
		perror("Could not set signal handler");
		return 1;
	}

    init_deck(deck);
    shuffle_deck(deck);
    for(int i = 0; i < 52; i++){
        printf("card: %c-%c\n", deck[i].numb, deck[i].symb);
    }

    pid_t child_pid; 
    char buffer[MAXLINE]; 
    char *message; 
    int len; 
    struct sockaddr_in servaddr, cliaddr; 
    int empezar;
    printf("Listening in port number: %d\n", PORT);

  
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(listenfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    

    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET;  

  
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 


    len = sizeof(cliaddr); 

    struct sockaddr_in connectedClients[100];
    int clientCount = 0;

    int n;
    while(1){
        n = recvfrom(listenfd, buffer, MAXLINE, 0, (struct sockaddr*)&cliaddr,&len); //receive message from server 
        
        int prevConnected = 0;
        for(int cc = 0; cc < clientCount; cc++){
            
            if(connectedClients[cc].sin_addr.s_addr != cliaddr.sin_addr.s_addr) continue;
            if(connectedClients[cc].sin_port != cliaddr.sin_port) continue;
            prevConnected = 1;
            break;
        }
        if(prevConnected == 0){
            connectedClients[clientCount] = cliaddr;
            clientCount += 1;
            printf("new connection\n");
        }

        child_pid = fork(); 

        if(child_pid==0){
            break; //breaking the cycle
        }

    }
    if(child_pid==0){//I'm the son
        if(n < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }else{
            buffer[n] = '\0'; 
            printf("\nHe recibido del cliente: ");
            printf("%s\n",buffer);
        }       
        for (int cc = 0; cc < clientCount; cc++){
            sendto(listenfd, buffer, strlen(buffer), 0, (struct sockaddr*)&connectedClients[cc], sizeof(connectedClients[cc])); 
        }
        close(listenfd);
        printf("Conexion cerrada por el cliente\n");               
    }else{
        close(fd);
        close(listenfd);
        printf("Conexion cerrada por el servidor\n");                    
    }
    
    return 0;
} 