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

#define MSG_SIZE 10000
#define PORT 5000

int sd, sd_actual; //descriptores de sockets
int addrlen; //longitud de msgecciones
struct sockaddr_in sind, pin; //msgecciones sockets cliente o servidor
			      //
			      //
char * readFile(){
    FILE *fptr;
    fptr = fopen("whiteboard.data", "r");

    char * data;
    data = malloc(sizeof(char) * MSG_SIZE);

    fgets(data, MSG_SIZE, fptr);
    fclose(fptr);

    return data;
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

    pid_t child_pid;
    while(1){
	if((sd_actual = accept(sd, (struct sockaddr *)&pin, &addrlen)) == -1){
	    perror("accept");
	    exit(1);
	}

	child_pid = fork();
	if(child_pid == 0){
	    break;
	}
	else{
	    close(sd_actual);
	}
    }
    if(child_pid==0){
	char * data;
	while(1){
	    if(recv(sd_actual, msg, sizeof(msg), 0) == -1){
		perror("recv");
		exit(1);
	    }

	    if(strcmp(msg, "close") == 0){
		break;
	    }
	    
	    if(strcmp(msg, "update") != 0){	
		writeFile(msg);
	    }

	    data = readFile();

	    printf("req TCP: %s\n", msg);
	    printf("res TCP: %s\n", data);

	    if(send(sd_actual, data, strlen(data), 0) == -1){
		perror("send");
		exit(1);
	    }
	}

	close(sd_actual);
	close(sd);
	printf("connexión cerrada\n");
    }
    else{
	close(sd);
    }
    printf("Conection closed \n");

    return 0;
}
