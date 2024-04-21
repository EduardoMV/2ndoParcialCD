#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <netdb.h>

#define UDP_PORT 8080
#define TCP_PORT 5000
#define MSG_SIZE 10000

int udp_listenfd, tcp_socketfd;

void abort_connection(int sig){
    printf("... closing server %d\n", sig);
    close(udp_listenfd);
    close(tcp_socketfd);
    exit(1);
}

int main(int argc, char* argv[])
{
    //TCP Client variables ----------------------------------------------------------------
    char tcp_dir[MSG_SIZE];
    int tcp_sd;
    struct hostent *tcp_hp;
    struct sockaddr_in tcp_sin, tcp_pin;
    int *tcp_status;
    char *tcp_host;

    //UDP Server Variables ----------------------------------------------------------------
    char udp_buffer[MSG_SIZE];
    char *udp_message = "Received";
    int udp_len;
    struct sockaddr_in udp_servaddr, udp_cliaddr;
    int udp_start;

    //ABORT CONECTION SETUP ---------------------------------------------------------------
    if(signal(SIGINT, abort_connection) == SIG_ERR){
	perror("Could not set signal handler");
	return 1;
    }
    //TCP Client Connection ---------------------------------------------------------------
    //parameter check
    if(argc != 2){
	fprintf(stderr, "TCP Error: %s <host>", argv[0]);
	exit(EXIT_FAILURE);
    }
    tcp_host = argv[1];

    if((tcp_hp = gethostbyname(tcp_host)) == 0){
	perror("TCP gethostname");
	exit(EXIT_FAILURE);
    }

    tcp_pin.sin_family = AF_INET;
    tcp_pin.sin_addr.s_addr = ((struct in_addr *) (tcp_hp -> h_addr)) -> s_addr;
    tcp_pin.sin_port = htons(TCP_PORT);

    if((tcp_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
	perror("TCP socket");
	exit(EXIT_FAILURE);
    }

    if(connect(tcp_sd, (struct sockaddr *)&tcp_pin, sizeof(tcp_pin)) == -1){
	perror("TCP connection");
	exit(EXIT_FAILURE);
    }

    strcpy(tcp_dir, "hello tcp");
    if(send(tcp_sd, tcp_dir, sizeof(tcp_dir), 0) == -1){
	perror("TCP send");
	exit(EXIT_FAILURE);
    }

    if(recv(tcp_sd, tcp_dir, sizeof(tcp_dir), 0) == -1){
	perror("TCP recv");
	exit(EXIT_FAILURE);
    }

    printf("%s\n", tcp_dir);
    
    //UDP Server Connection ---------------------------------------------------------------
    udp_listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_listenfd == -1){
	perror("UDP socket creation failed");
	exit(EXIT_FAILURE);
    }

    udp_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    udp_servaddr.sin_port = htons(UDP_PORT);
    udp_servaddr.sin_family = AF_INET;

    bind(udp_listenfd, (struct sockaddr*)&udp_servaddr, sizeof(udp_servaddr));
    udp_len = sizeof(udp_cliaddr);

    //Handleling multiple connections
    pid_t child_pid;
    int n;
    while(1){
	n = recvfrom(udp_listenfd, udp_buffer, MSG_SIZE, 0, (struct sockaddr*)&udp_cliaddr, &udp_len);
	child_pid = fork();

	if(child_pid == 0){
	    break;
	}
    }

    if(child_pid == 0){
	//executes for each client
	if(n < 0){
	    perror("UDP recvfrom failed");
	    exit(EXIT_FAILURE);
	}else{
	    udp_buffer[n] = '\0';
	    printf("client msg: %s\n", udp_buffer);
	    
	    strcpy(tcp_dir, udp_buffer);
	    if(send(tcp_sd, tcp_dir, sizeof(tcp_dir), 0) == -1){
		perror("TCP send");
		exit(EXIT_FAILURE);
	    }

	    memset(tcp_dir, '\0', sizeof(tcp_dir));

	    if(recv(tcp_sd, tcp_dir, sizeof(tcp_dir), 0) == -1){
		perror("TCP recv");
		exit(EXIT_FAILURE);
	    }
	    
	}

	printf("from TCP: %s\n", tcp_dir);

	sendto(udp_listenfd, tcp_dir, strlen(tcp_dir), 0, (struct sockaddr*)&udp_cliaddr, sizeof(udp_cliaddr));
	close(udp_listenfd);
	printf("Connection close by Client UDP\n");

    }else{
	close(udp_listenfd);
	printf("Connection closed by Server UDP\n");
    }

    close(tcp_sd); //close tcp connection
    return 0;
}
