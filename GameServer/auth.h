//
// Created by EUMV on 20/04/2024.
//

#ifndef LOGIN_H
#define LOGIN_H

#include "csvHandler.h"

int login (const char *user, const char *pass) {
    FILE *fptr;
    fptr = fopen("usuarios.csv", "r");
    
    char line[1024];

    while(fgets(line, 1024, fptr)){
        char *line_user = strtok(line, ","); 
        char *line_pass = strtok(NULL, ",");

        if(strcmp(user, line_user) == 0 && strcmp(pass, line_pass) == 0){
            return 1;
        } 
    }
    return 0;
}

int sign_up (const char *user, const char *pass) {
    if(login(user, pass) == 1){
        return 1;
    }

    FILE *file = fopen("usuarios.csv", "a");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return 0;
    }

    fprintf(file, "%s,%s\n", user, pass);
    fclose(file);
    return 1;
}

#endif /* LOGIN_H */