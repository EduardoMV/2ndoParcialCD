//
// Created by EUMV on 20/04/2024.
//

#ifndef LOGIN_H
#define LOGIN_H

#include "csvHandler.h"

int login (const char *user, const char *pass) {
    printf("login-func\n");
    FILE *fptr;
    fptr = fopen("usuarios.csv", "r");
    
    char line[1024];
    printf("starting to read file\n");
    while(fgets(line, 1024, fptr)){
        char *line_user = strtok(line, ","); 
        char *line_pass = strtok(NULL, ",");

        printf("%s == %s && %s == %s \n", user, line_user, pass, line_pass);
        if(strcmp(user, line_user) == 0 && strcmp(pass, line_pass) == 0){
            printf("match\n");
            return 1;
        } 
        else{
            printf("diff_user: %d\n", strcmp(user,line_user));
            printf("diff_pass: %d\n", strcmp(pass,line_pass));

        }
    }
    return 0;
    /*

    int file = load_csv("usuarios.csv", &data, &rows, &cols);

    printf("vars\n");

    if (load_csv("usuarios.csv", &data, &rows, &cols) == -1) {
        printf("Error al leer el archivo de usuarios\n");
        return 0;
    }

    printf("rows: %d\tcols: %d", rows, cols);

    for (int i = 0; i < rows; i++) {
        printf("comparing users: %s, %s\n", user, data[i][0]);
        printf("comparing passwords: %s, %s\n", pass, data[i][1]);

        if (strcmp(user, data[i][0]) == 0 && strcmp(pass, data[i][1]) == 0) {
            return 1; // Login exitoso
        }
    }
    */
}

int sign_up (const char *user, const char *pass) {
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