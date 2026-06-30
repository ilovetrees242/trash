#include <stdio.h>

void logErr(const char* msg){
    fprintf(stderr, "\033[1;31merror: \033[0m%s\n", msg);
}
