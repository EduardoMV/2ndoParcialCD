#ifndef LOGIN_H
#define LOGIN_H

#include "csv_handler.h"

// Función para realizar el login
int login(const char *user, const char *pass) {
    double **data;
    int rows = 0, cols = 0;

    if (read_csv("usuarios.csv", &data, &rows, &cols) == -1) {
        printf("Error al leer el archivo de usuarios\n");
        return 0;
    }

    for (int i = 0; i < rows; i++) {
        if (strcmp(user, data[i][0]) == 0 && strcmp(pass, data[i][1]) == 0) {
            return 1; // Login exitoso
        }
    }

    return 0; // Usuario no encontrado o contraseña incorrecta
}

int add_user(const char *user, const char *pass) {
    FILE *file = fopen("usuarios.csv", "a");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    fprintf(file, "%s,%s\n", user, pass);
    fclose(file);
    return 0;
}

#endif /* LOGIN_H */
