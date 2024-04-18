/*

   Conexion UDP
   Codigo del servidor

   Nombre Archivo: udpserver.c   
   Fecha: Febrero 2023

   Compilacion: cc udpserver.c -lnsl -o udpserver
   EjecuciĆ³n: ./udpserver

*/

// server program for udp connection 
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define PORT 8080
#define MAXLINE 1000

int listenfd;

void abort_connection(int sig){
    printf("... closing server %d\n", sig);
    close(listenfd);
    exit(1);
}

// Driver code
int main()
{
    char buffer[MAXLINE];
    char *message = "Received";
    int len;
    struct sockaddr_in servaddr, cliaddr;
    int empezar;

    if(signal(SIGINT, abort_connection) == SIG_ERR){
        perror("Could not set signal handler");
        return 1;
    }
    printf("Listening on port number: %d\n", PORT);

    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(listenfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    // bind server address to socket descriptor
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //receive the datagram
    while(1){
        len = sizeof(cliaddr);
        int n = recvfrom(listenfd, buffer, MAXLINE, 0, (struct sockaddr*)&cliaddr,&len); //receive message from client
        if(n < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }else{
            buffer[n] = '\0';
            printf("\nReceived from client: ");
            printf("%s\n",buffer);
        }

        // Fork a new process to handle the client request
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            // send the response from server
            sendto(listenfd, message, strlen(message), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
            exit(EXIT_SUCCESS); // Exit child process
        } else { // Parent process
            // Close the socket in the parent process, as it's not needed
            close(listenfd);
        }
    }
    close(listenfd);
    printf("Connection closed\n");
    return 0;
} 
