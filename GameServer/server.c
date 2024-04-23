
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
#define MAX_USERS 10

struct User {
    char username[100];
};

struct User users[MAX_USERS]; // Lista de usuarios
int userCount = 0;

int listenfd;
int fd;
struct card deck[52];
char action[100];
char data[100];
char userlist[MAXLINE] = "";


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
    
    for(int i = 0; i < 100; i++){
        struct card curr = takeCard(deck);
        printf("%c-%c\n", curr.numb, curr.symb);
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
    
    char *users[10];
    int userCount = 0; 

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
        sscanf(buffer, "game:user=%[^&]&action=%[^&]&data=%s", username, action, data);

        if (strcmp(action, "game") == 0) {
            if (strcmp(action, "join") == 0) {
                if (userCount >= MAX_USERS) {
                    strcpy(buffer, "game:action=join&data=\"match full\"");
                    sendto(listenfd, buffer, strlen(buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
                } else {

                    int userExists = 0;
                    for (int i = 0; i < userCount; i++) {
                        if (strcmp(username, users[i].username) == 0) {
                            userExists = 1;
                            break;
                        }
                    }

                    if (userExists) {
                        strcpy(buffer, "game:action=join&data=\"user already joined\"");
                    } else {
                        if (userCount >= MAX_USERS) {
                            strcpy(buffer, "game:action=join&data=\"match full\"");
                        } else {

                            strcpy(users[userCount].username, username);
                            userCount++;
                            strcpy(buffer, "game:action=join&data=");

                            for (int i = 0; i < userCount; i++) {
                                strcat(buffer, users[i].username);
                                strcat(buffer, ",");
                            }

                            if (strlen(buffer) > 0) {
                                buffer[strlen(buffer) - 1] = '\0';
                            }
                        }
                    }
                    sendto(listenfd, buffer, strlen(buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
                }
            } else if (strcmp(action, "start") == 0) {
                // Verifica si todos los jugadores han enviado "start"
                // Si es así, inicia el juego
                // Envía la respuesta al primer jugador para comenzar el juego
            } else if (strcmp(action, "take") == 0) {
                // Lógica para que el usuario tome una carta
                // Asegúrate de que la carta no se repita y de que se envíe al usuario correcto
                // Envía la respuesta al usuario con la carta tomada
            } else if (strcmp(action, "endTurn") == 0) {
                // Lógica para pasar al siguiente turno
                // Asegúrate de manejar correctamente el caso en el que ya sea el último turno
                // Envía la respuesta al siguiente jugador
            } else if (strcmp(action, "dealer") == 0) {
                // Lógica para devolver el valor del dealer (cartas)
                // Envía la respuesta al cliente con las cartas del dealer
            }
        } else {

        }

        //cliente
        //game:user=BraulioSg&action=join; -> agregar jugador       -> game:to=everyone&data=<jugador1>,<jugador2>,<jugador3>....
        //game:user=<usuario>&action=take&; -> regresar una carta    -> game:to=<user>&data=9-C

        //game:user=<usuario>&action=<action>&data=<data || null>
        
        /*
        ACTIONS:
        | usuario | -> | accion | -> | respuesta |
           join     ->  agregarlo ->  la lista de jugadores conectados (incluyendolo) game:action=join&data=<jugador1>,<jugador2>,<jugador3>....; //NOTA: No puede haber jugadores repeditos;
           
           start    ->  verifica si todos los conectados mandaron start -> game:action=start&data=<primer jugador>,

           take     ->  devolver una carta ->  game:action=take&data=9-C&to=<usuario>
           
           endTurn  ->  regresas el siguiente turno -> game:action=endTurn&data=<siguiente jugador>
                        si ya fue el ultimo data=null;
           dealer(el valor que tiene) -> regresar -> game:action=dealer&data=<carta1,carta2,carta3....>         



        -> join: agregar usuario
        -> start: empezar el juego (todos los jugadores tuvieron que mandar esto);
        -> take: agarra una carte
        -> startTurn: el jugador empezó el turno
        -> endTurn: terminó el turno 
        */


        //chat:user=BraulioSG&msg="hello world"
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