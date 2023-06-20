#include <stdio.h>
#include "loadfile.h"
#include <string.h>

unsigned char code[10000];

int main(int argc, char** argv) {
    if(loadFile(argv[1], code)) {
        printf("Failed to open code file \"%s\"!\n", argv[1]);
        return 1;
    }
    char* token = strtok(code, "\n");
    while (token != NULL) {
        // do something with the token
        printf("%s\n", token);
        token = strtok(NULL, "\n");
    }
    return 0;
}