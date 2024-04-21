#include <stdio.h>
/* The following headers was required in older or some compilers*/
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <signal.h> // it is required to call signal handler functions
#include <unistd.h> // it is required to close the socket descriptor
#include "auth.h"

#define DIRSIZE 2048 /* longitud maxima parametro entrada/salida */
#define PUERTO 5000	 /* numero puerto arbitrario */

int sd, sd_actual;			  /* descriptores de sockets */
int addrlen;				  /* longitud direcciones */
struct sockaddr_in sind, pin; /* direcciones sockets cliente u servidor */

/*  procedimiento de aborte del servidor, si llega una senal SIGINT */
/* ( <ctrl> <c> ) se cierra el socket y se aborta el programa       */
void aborta_handler(int sig)
{
	printf("....abortando el proceso servidor %d\n", sig);
	close(sd);
	close(sd_actual);
	exit(1);
}

void sendMsg(char *msg){
	char dir[DIRSIZE];

	strcpy(dir, msg);
	if (send(sd_actual, dir, strlen(dir), 0) == -1)
	{
		perror("send");
		exit(1);
	}
}

int loginUser(char *data){
	char dir[DIRSIZE];

	if(data == NULL) return 0;

	char *userField = strtok(data, "&");
	char *passField = strtok(NULL, "&");

	char *userLabel = strtok(userField, "=");
	char *userValue = strtok(NULL, "=");

	char *passLabel = strtok(passField, "=");
	char *passValue = strtok(NULL, "=");

	//returs 1 if the user and password are correct in the csv
	return login(userValue, passValue);
}

int signupUser(char *data){
	char dir[DIRSIZE];

	if(data == NULL) return 0;

	char *userField = strtok(data, "&");
	char *passField = strtok(NULL, "&");

	char *userLabel = strtok(userField, "=");
	char *userValue = strtok(NULL, "=");

	char *passLabel = strtok(passField, "=");
	char *passValue = strtok(NULL, "=");

	//returs 1 if the user and password are correct in the csv
	return signup(userValue, passValue);

}

int main()
{

	char dir[DIRSIZE];

	if (signal(SIGINT, aborta_handler) == SIG_ERR)
	{
		perror("Could not set signal handler");
		return 1;
	}

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	sind.sin_family = AF_INET;
	sind.sin_addr.s_addr = INADDR_ANY; /* INADDR_ANY=0x000000 = yo mismo */
	sind.sin_port = htons(PUERTO);	   /*  convirtiendo a formato red */

	/* asociando el socket al numero de puerto */
	if (bind(sd, (struct sockaddr *)&sind, sizeof(sind)) == -1)
	{
		perror("bind");
		exit(1);
	}

	/* ponerse a escuchar a traves del socket */
	if (listen(sd, 5) == -1)
	{
		perror("listen");
		exit(1);
	}

	int max = 4;
	pid_t child_pid;
	while (1)
	{ // Maximun max client connections
		/* esperando que un cliente solicite un servicio */
		if ((sd_actual = accept(sd, (struct sockaddr *)&pin,
								&addrlen)) == -1)
		{
			perror("accept");
			exit(1);
		}
		child_pid = fork();
		if (child_pid == 0)
		{
			// soy el hijo
			break;
		}
		else
		{
			// soy el padre
			close(sd_actual);
		}
	}
	if (child_pid == 0)
	{
		// soy el hijo
		/* tomar un mensaje del cliente */
		if (recv(sd_actual, dir, sizeof(dir), 0) == -1)
		{
			perror("recv");
			exit(1);
		}

		char *token = strtok(dir, ":");
		
		if(strcmp(token, "login") == 0){
			int auth = loginUser(strtok(NULL, ""));
			if(auth){
				sendMsg("success");
			}else{
				sendMsg("failed");
			}
		}
		
		if(strcmp(token, "signup") == 0){
			int auth = signupUser(strtok(NULL, ""));
			if(auth){
				sendMsg("success");
			}else{
				sendMsg("failed");
			}
		}

		/* cerrar los dos sockets */
		close(sd_actual);
		close(sd);
		printf("Conexion cerrada\n");
	}
	else
	{
		// soy el padre
		close(sd);
	}
	return 0;
}