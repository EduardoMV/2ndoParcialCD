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

        if(line_pass[strlen(line_pass) - 1] == '\n') line_pass[strlen(line_pass) - 1] = '\0';

        if(strcmp(user, line_user) == 0 && strcmp(pass, line_pass) == 0){
            fclose(fptr);
            return 1;
        } 
    }
    fclose(fptr);
    return 0;
}

int sign_up (const char *user, const char *pass) {
    FILE *fptr;
    fptr = fopen("usuarios.csv", "r");
    
    char line[1024];

    while(fgets(line, 1024, fptr)){
        char *line_user = strtok(line, ","); 

        if(strcmp(user, line_user) == 0){
            fclose(fptr);
            return 0;
        } 
    }
    fclose(fptr);

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