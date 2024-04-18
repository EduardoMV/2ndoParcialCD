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

#define UDP_PORT 8080
#define TCP_PORT 8888
#define MAXLINE 1000

int udp_listenfd;
int tcp_socketfd;

void abort_connection(int sig){
    printf("... closing server %d\n", sig);
    close(udp_listenfd);
    close(tcp_socketfd);
    exit(1);
}

int main()
{
    char buffer[MAXLINE];
    char *message = "Received";
    int len;
    struct sockaddr_in udp_servaddr, cliaddr, tcp_servaddr;
    int empezar;

    if(signal(SIGINT, abort_connection) == SIG_ERR){
        perror("Could not set signal handler");
        return 1;
    }
    printf("UDP Server listening on port number: %d\n", UDP_PORT);
    printf("Connecting to TCP Server on port number: %d\n", TCP_PORT);

    // Create a UDP Socket
    udp_listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_listenfd == -1) {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    udp_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    udp_servaddr.sin_port = htons(UDP_PORT);
    udp_servaddr.sin_family = AF_INET;

    // bind UDP server address to socket descriptor
    if(bind(udp_listenfd, (struct sockaddr*)&udp_servaddr, sizeof(udp_servaddr)) != 0) {
        perror("UDP bind failed");
        exit(EXIT_FAILURE);
    }

    // Create a TCP Socket
    tcp_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_socketfd == -1) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    tcp_servaddr.sin_family = AF_INET;
    tcp_servaddr.sin_port = htons(TCP_PORT);
    tcp_servaddr.sin_addr.s_addr = INADDR_ANY;

    // Connect to TCP server
    if (connect(tcp_socketfd, (struct sockaddr *)&tcp_servaddr, sizeof(tcp_servaddr)) < 0) {
        perror("TCP server connection failed");
        exit(EXIT_FAILURE);
    }

    //receive the datagram
    while(1){
        len = sizeof(cliaddr);
        int n = recvfrom(udp_listenfd, buffer, MAXLINE, 0, (struct sockaddr*)&cliaddr,&len); //receive message from client
        if(n < 0) {
            perror("UDP recvfrom failed");
            exit(EXIT_FAILURE);
        }else{
            buffer[n] = '\0';
            printf("\nReceived from client: ");
            printf("%s\n",buffer);

            // Send the received data to the TCP server for storage
            send(tcp_socketfd, buffer, strlen(buffer), 0);
        }
        // send the response from server
        sendto(udp_listenfd, message, strlen(message), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    }
    close(udp_listenfd);
    close(tcp_socketfd);
    printf("Connection closed\n");
    return 0;
}

