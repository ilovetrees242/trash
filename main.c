#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "builtins.h"

char *cmd;
char *args[128] = {};
char *tok = "";
char buffer[128] = "";

void splitArgs(const char data[]){
    if ( strcmp(data, "") == 0 ) return;
    int counter = 0;
    const char delimiters[] = " ";
    strcpy(buffer, data);
    tok = strtok(buffer, delimiters);
    for(int i = 0; i < 128; i++){
        args[i] = NULL;
    }
    while (tok != NULL){
        args[counter] = tok;
        counter++;
        tok = strtok(NULL, delimiters);
    }
    cmd = args[0];
}
int main(){
    char *input;
    char wd[256];
    char prompt[384] = "";
    struct passwd *pw = getpwuid(geteuid());
    const char* user = pw->pw_name;
    const char* homedir = pw->pw_dir;
    while ( true ) {
        getcwd(wd, 256);
        if (strstr(wd, homedir) != NULL){
            char buffer[256];
            wd[0] = '~';
            strncpy(buffer, wd + strlen(homedir), strlen(wd));
            for(int i = 1; i < 256 - 1; i++){
                wd[i] = '\0';
            }
            strcat(wd, buffer);
        }
        snprintf(prompt, sizeof(prompt), "[ %s | %s ]$ ", user, wd);
        input = readline(prompt);
        if ( input == NULL )
            break;
        if ( input == "" )
            continue;
        if ( input != NULL )
            add_history(input);

        splitArgs(input);

        bool bRan = runBuiltIn(cmd, args);
        if ( bRan ) continue;

        pid_t pid = fork();
        if ( pid == 0 ){
            execvp(cmd, args);
            if (errno == 2){
                printf("\033[1;31merror: \033[0;37mcommand not found -> %s\n", cmd);
                exit(127);
            }
        }
        else {
            wait(NULL);
        }
        free(input);
        memset(prompt, '\0',sizeof(prompt));
    }
}
