//
// Created by EUMV on 20/04/2024.
//

#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// - filename: nombre del archivo CSV
// - data: matriz de datos a guardar
// - rows: número de filas en la matriz de datos
// - cols: número de columnas en la matriz de datos
// - 0 si la operación fue exitosa, -1 si ocurrió un error

/*
int save_csv (const char *filename, char ***data, int rows, int cols) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%s", data[i][j]);
            if (j < cols - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}
*/
/*
int load_csv (const char *filename, char ***data, int *rows, int *cols) {
    printf("read_csv function");
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("file is null\n");
        perror("Error al abrir el archivo");
        return -1;
    }

    char line[1024];
    int r = 0;
    printf("start reading the file");
    while (fgets(line, sizeof(line), file)) {
        char *token;
        token = strtok(line, ",");
        int c = 0;
        printf("line");
        while (token != NULL) {
            printf("column");
            strcpy((*data)[r][c++], token);
            token = strtok(NULL, ",");
        }
        (*rows)++;
        r++;
    }
    *cols = (*rows > 0) ? (r / (*rows)) : 0;

    fclose(file);
    return 0;
}
*/

#endif /* CSV_HANDLER_H */