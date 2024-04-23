#ifndef GAME_H
#define GAME_H

char* join_strings(char* strings[], char* seperator, int count) {
    char* str = NULL;             /* Pointer to the joined strings  */
    size_t total_length = 0;      /* Total length of joined strings */
    int i = 0;                    /* Loop counter                   */

    /* Find total length of joined strings */
    for (i = 0; i < count; i++) total_length += strlen(strings[i]);
    total_length++;     /* For joined string terminator */
    total_length += strlen(seperator) * (count - 1); // for seperators

    str = (char*) malloc(total_length);  /* Allocate memory for joined strings */
    str[0] = '\0';                      /* Empty string we can append to      */

    /* Append all the strings */
    for (i = 0; i < count; i++) {
        strcat(str, strings[i]);
        if (i < (count - 1)) strcat(str, seperator);
    }

    return str;
}


int userInMatch(const char *user) {
    FILE *fptr;
    fptr = fopen("game.log", "r");
    
    char line[1024];

    while(fgets(line, 1024, fptr)){
        line[strcspn(line, "\n")] = '\0'; //removes new line

        char *line_user = strtok(line, "="); 


        if(strcmp(user, line_user) == 0){
            fclose(fptr);
            return 0;
        } 

    }
    fclose(fptr);
    return -1;
}


int addNewPlayer(char *user){
    if(userInMatch(user) == 0) return 0;

    FILE *file = fopen("game.log", "a");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    fprintf(file, "%s=0\n", user);
    fclose(file);
    return 0;
}

char* getAllPlayers(){
    FILE *file = fopen("game.log", "r");
    if (file == NULL) {
        printf("Error opening file %s\n", "game.log");
        return NULL;
    }

    // Find the length of the file
    fseek(file, 0, SEEK_END);
    long file_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the string
    char *concatenated_users = (char *)malloc((file_length + 1) * sizeof(char));
    if (concatenated_users == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    // Read each line and concatenate users
    char line[256]; // Assuming no line will be longer than 255 characters
    concatenated_users[0] = '\0'; // Ensure the string starts empty
    while (fgets(line, sizeof(line), file) != NULL) {
        char *user = strtok(line, "="); // Split the line by '='
        if (user != NULL) {
            strcat(concatenated_users, user);
            strcat(concatenated_users, ",");
        }
    }

    // Remove the trailing comma
    if (strlen(concatenated_users) > 0) {
        concatenated_users[strlen(concatenated_users) - 1] = '\0';
    }

    fclose(file);
    return concatenated_users;
}

int reset_file(){
    FILE *file = fopen("game.log", "w"); 
    
    if (file == NULL) {
        printf("Error opening file %s\n", "game.log");
        return 0;
    }
    fclose(file);
    return 1;
}

void update_player_start(const char *user, int start) {
    FILE *file = fopen("game.log", "r+");
    if (file == NULL) {
        printf("Error opening file %s\n", "game.log");
        return;
    }

    char line[256]; // Assuming no line will be longer than 255 characters
    char *found_user;
    long pos;

    // Iterate through each line in the file
    while (fgets(line, sizeof(line), file) != NULL) {
        found_user = strstr(line, user); // Check if the line contains the user
        if (found_user != NULL) {
            // Update the value for the user
            pos = ftell(file) - strlen(line);
            fseek(file, pos, SEEK_SET);
            fprintf(file, "%s=%d\n", user, start);
            break; // No need to continue once the user is found and updated
        }
    }

    fclose(file);
}


int all_users_started(){
    FILE *fpr = fopen("game.log","r");

    char line[1024];

    int all_started = 1;

    while(fgets(line, sizeof(line), fpr) != NULL){
        line[strcspn(line, "\n")] = '\0'; //removes new line
        char *num = strtok(line, "=");
        num = strtok(NULL, "=");

        printf("num: %s\n", num);

        if(strcmp(num, "0") == 0) {
            all_started = 0;
        }
    }
    return all_started;
}

#endif /* GAME_H */