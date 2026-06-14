#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>

char *cmd;
char mData[256];
char *args[128] = {};
char *tok = "";

void splitArgs(const char data[]){
    if ( strcmp(data, "") == 0 ) return;
    int counter = 0;
    char buffer[128] = "";
    const char delimiters[] = " ";
    strcpy(mData, data);
    tok = strtok(mData, delimiters);
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
bool runBuiltIn(const char *cmd){
    struct passwd *pw = getpwuid(geteuid());
    const char *homedir = pw->pw_dir;
    if ( strcmp(cmd, "exit") == 0 ){
        if ( args[1] != NULL ) {
            char *endp;
            int val = strtol(args[1], NULL, 10);
            exit(val);
        }
        else {
            exit(0);
        }
    }
    else if ( strcmp(cmd, "cd") == 0 ){
        if ( args[1] != NULL )
            if (args[1][0] == '~'){
                char buffer[256];
                char *arg = args[1];
                strncpy(arg, args[1] + 1, strlen(arg));
                strcat(buffer, homedir);
                strcat(buffer, arg);
                chdir(buffer);
            }
            else
                chdir(args[1]);
        else
            chdir(homedir);
        return true;
    }
    else
        return false;
}
int main(){
    char input[50] = "";
    char wd[256];
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
        printf("\033[1;33m[%s] %s\033[1;37m > \033[0m", user, wd);
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';
        if ( strcmp("", input) != 0 ){
            splitArgs(input);
            bool bRan = runBuiltIn(cmd);
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
        }
    }
}
