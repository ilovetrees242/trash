#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>

char cmd[256] = "";
char *args[128] = {};

void splitArgs(const char *data){
    int counter = 0;
    int argCounter = 0;
    char buffer[128] = "";
        printf("%s\n", args[0]);
    for(int i = 0; i < strlen(data); i++){
        if ( data[i] == ' ' ){
            args[argCounter] = buffer;
            argCounter++;
            strcpy(buffer, "");
            counter = 0;
            continue;
        }
        buffer[counter] = data[i];
        counter++;
    }
}
bool runBuiltIn(const char *cmd){
    int builtIn = strcmp(cmd, "exit");
    if ( builtIn == 0 ){
        exit(0);
    }
}
int main(){
    char input[50] = "";
    while ( true ) {
        printf("> ");
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';
        runBuiltIn(input);
        pid_t pid = fork();
        if ( pid == 0 ){
            execvp(input, NULL);
        }
        else {
            wait(NULL);
        }
    }
}
