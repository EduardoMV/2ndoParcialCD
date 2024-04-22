//
// Created by EUMV on 20/04/2024.
//

#ifndef LOGIN_H
#define LOGIN_H

#include "csvHandler.h"

void get_user_data(char *dest, const char *user){
    FILE *fptr;
    fptr = fopen("usuarios.csv", "r");

    char line[1024];
    char data[1024];

    while(fgets(line, 1024, fptr)){
        line[strcspn(line, "\n")] = '\0'; //removes new lin
        strcpy(data, line);

        char *line_user = strtok(line, ","); 



        if(strcmp(user, line_user) == 0){
            fclose(fptr);
            strcpy(dest, data);
            return;
        } 
    }
    fclose(fptr);
    strcpy(dest, "null"); 
    return;
}

int login (const char *user, const char *pass) {
    FILE *fptr;
    fptr = fopen("usuarios.csv", "r");
    
    char line[1024];

    while(fgets(line, 1024, fptr)){
        line[strcspn(line, "\n")] = '\0'; //removes new line

        char *line_user = strtok(line, ","); 
        char *line_pass = strtok(NULL, ",");


        if(strcmp(user, line_user) == 0 && strcmp(pass, line_pass) == 0){
            fclose(fptr);
            return 1;
        } 

    }
    fclose(fptr);
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

    fprintf(file, "%s,%s,%d\n", user, pass,100);
    fclose(file);
    return 1;
}

#endif /* LOGIN_H */