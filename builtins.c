#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

bool runBuiltIn(const char *cmd, char *args[]){
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
        if ( args[1] != NULL ){
            if (args[1][0] == '~'){
                char buffer[256] = "";
                strcat(buffer, homedir);
                strcat(buffer, args[1] + 1);
                chdir(buffer);
            }
            else
                chdir(args[1]);
        }
        else
            chdir(homedir);
        return true;
    }
    else
        return false;
}
