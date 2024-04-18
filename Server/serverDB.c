#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <time.h>

#define MSG_SIZE 1024
#define PORT 5000

int sd, sd_actual; //descriptores de sockets
int addrlen; //longitud de msgecciones
struct sockaddr_in sind, pin; //msgecciones sockets cliente o servidor
			      //
void readFile(){
    FILE *fptr;
    fptr = fopen("whiteboard.data", "r");

    char data[1024];

    fgets(data, 1024, fptr);
    fclose(fptr);

    printf("%s\n", data);

}

void writeFile(char text[]){
    FILE *fptr;
    fptr = fopen("whiteboard.data", "w");
    fprintf(fptr, "%s",text);
    fclose(fptr);
}

void abort_connection(int sig){
    printf("...closing server %d\n", sig);
    close(sd);
    close(sd_actual);
}

int main(){
    char msg[MSG_SIZE];

    //set up shutdown connection when ctrl + c
    if(signal(SIGINT, abort_connection) == SIG_ERR){
	perror("Could not set signal handler");
	return 1;
    }

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Error while getting the socket");
	exit(1);
    }

    sind.sin_family = AF_INET;
    sind.sin_addr.s_addr = INADDR_ANY;
    sind.sin_port = htons(PORT);

    if(bind(sd, (struct sockaddr *)&sind, sizeof(sind)) == -1){
	perror("Error while binding");
	exit(1);
    }

    if(listen(sd, 5) == -1){
	perror("Error at listening");
	exit(1); 
    }

    if((sd_actual = accept(sd, (struct sockaddr *)&pin, &addrlen)) == -1){
	perror("Error on accepting connection");
	exit(1);
    }
    
    int keepGoing = 1; 

    while(keepGoing){
	int n = recv(sd_actual, msg, sizeof(msg), 0);
	if(n == -1){
	    perror("Error on recieving");
	    exit(1);
	}
	msg[n] = '\0';
	writeFile(msg);
	readFile();
	printf("message stored\n");

	if(strcmp(msg, "close") == 0){
	    keepGoing = 0; 
	}
	int sent = send(sd_actual, "saved", strlen("saved"), 0);
	if(sent == -1){
	    perror("Error on sent");
	    exit(1);
	}
    }

    close(sd_actual);
    close(sd);
    printf("Conection closed \n");

    return 0;
}
