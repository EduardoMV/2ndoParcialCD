#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define MAX_CLIENTS 2
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int server_socket, client_socket[MAX_CLIENTS], max_clients = MAX_CLIENTS, activity, i, valread;
    int max_sd;
    struct sockaddr_in server_addr;

    char buffer[BUFFER_SIZE];

    // Inicializar el arreglo de sockets de clientes
    for (i = 0; i < max_clients; i++) {
        client_socket[i] = 0;
    }

    // Crear socket del servidor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error("Error al crear el socket del servidor");
    }

    // Configurar la estructura de la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Asignar una dirección al socket del servidor
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Error al asignar una dirección al socket del servidor");
    }

    // Escuchar por conexiones entrantes
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        error("Error al escuchar por conexiones entrantes");
    }

    printf("Servidor escuchando en el puerto %d...\n", PORT);

    while (1) {
        fd_set readfds;

        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);
        max_sd = server_socket;

        for (i = 0; i < max_clients; i++) {
            int sd = client_socket[i];

            if (sd > 0) {
                FD_SET(sd, &readfds);
            }

            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // Esperar por alguna actividad en algún socket
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            error("Error en select");
        }

        // Verificar si hay una nueva conexión entrante
        if (FD_ISSET(server_socket, &readfds)) {
            int new_socket;
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);

            if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
                error("Error al aceptar una nueva conexión");
            }

            printf("Nuevo cliente conectado, socket fd: %d, IP: %s, Puerto: %d\n", new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Añadir el nuevo socket al arreglo de sockets de clientes
            for (i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }

        // Verificar las operaciones en los sockets de clientes
        for (i = 0; i < max_clients; i++) {
            int sd = client_socket[i];

            if (FD_ISSET(sd, &readfds)) {
                // Leer el mensaje del cliente
                if ((valread = read(sd, buffer, BUFFER_SIZE)) == 0) {
                    // Desconexión de un cliente
                    getpeername(sd, (struct sockaddr *)&server_addr, (socklen_t *)&addr_len);
                    printf("Cliente desconectado, IP: %s, Puerto: %d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
                    close(sd);
                    client_socket[i] = 0;
                } else {
                    // Me falta implementar la lógica del pinturillo
                    // Por ahora, solo mostramos el mensaje recibido
                    buffer[valread] = '\0';
                    printf("Mensaje del cliente %d: %s\n", sd, buffer);

                    // Por ejemplo, podriamos enviar el mensaje a todos los clientes conectados
                    // para que sepan lo que el primer cliente está dibujando.

                    // Enviar un mensaje de ejemplo al cliente de Node.js
                    char *message = "Hola desde el servidor C";
                    send(sd, message, strlen(message), 0);
                }
            }
        }
    }

    return 0;
}