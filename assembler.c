#include <stdio.h>
#include "loadfile.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[100];
    unsigned short address;
} Label;

unsigned char code[10000];
unsigned char machineCode[0x10000];
unsigned short machineCodeIndex = 0;
Label labelTable[100];
int labelIndex = 0;

unsigned short findAddress(char* token) {
    for (int i = 0; i < labelIndex; i++) {
        if (strcmp(labelTable[i].name, token) == 0) {
            return labelTable[i].address;
        }
    }
    char* endptr;
    int address = strtol(token, &endptr, 0);
    if (*endptr != '\0') {
        printf("WARNING: token \"%s\" is not a label or number\n", token);
    }
    return address;
}

int main(int argc, char** argv) {
    if(loadFile(argv[1], code)) {
        printf("Failed to open code file \"%s\"!\n", argv[1]);
        return 1;
    }
    char* savedLine = NULL;
    char* line = strtok_r(code, "\n", &savedLine);
    for (; line != NULL; line = strtok_r(NULL, "\n", &savedLine)) {
        char* opcode = strtok(line, " ");
        if (strcmp(opcode, "load") == 0) {
            // immidiate
            char* token2 = strtok(NULL, " ");
            if (strcmp(token2, "#") == 0) {
                machineCode[machineCodeIndex] = 1; machineCodeIndex++;
                machineCode[machineCodeIndex] = strtol(strtok(NULL, " "), NULL, 0); machineCodeIndex++;
            }
            // memory
            else {
                machineCode[machineCodeIndex] = 2; machineCodeIndex++;
                unsigned short address = findAddress(token2);
                machineCode[machineCodeIndex] = address; machineCodeIndex++;
                machineCode[machineCodeIndex] = address >> 8; machineCodeIndex++;
            }
            continue;
        }
        if (strcmp(opcode, "add") == 0) {
            // immidiate
            char* token2 = strtok(NULL, " ");
            if (strcmp(token2, "#") == 0) {
                machineCode[machineCodeIndex] = 3; machineCodeIndex++;
                machineCode[machineCodeIndex] = strtol(strtok(NULL, " "), NULL, 0); machineCodeIndex++;
            }
            // memory
            else {
                machineCode[machineCodeIndex] = 4; machineCodeIndex++;
                unsigned short address = findAddress(token2);
                machineCode[machineCodeIndex] = address; machineCodeIndex++;
                machineCode[machineCodeIndex] = address >> 8; machineCodeIndex++;
            }
            continue;
        }
        if (strcmp(opcode, "sub") == 0) {
            // immidiate
            char* token2 = strtok(NULL, " ");
            if (strcmp(token2, "#") == 0) {
                machineCode[machineCodeIndex] = 5; machineCodeIndex++;
                machineCode[machineCodeIndex] = strtol(token2, NULL, 0); machineCodeIndex++;
            }
            // memory
            else {
                machineCode[machineCodeIndex] = 6; machineCodeIndex++;
                unsigned short address = findAddress(strtok(NULL, " "));
                machineCode[machineCodeIndex] = address; machineCodeIndex++;
                machineCode[machineCodeIndex] = address >> 8; machineCodeIndex++;
            }
            continue;
        }
        if (strcmp(opcode, "store") == 0) {
            machineCode[machineCodeIndex] = 7; machineCodeIndex++;
            unsigned short address = findAddress(strtok(NULL, " "));
            machineCode[machineCodeIndex] = address; machineCodeIndex++;
            machineCode[machineCodeIndex] = address >> 8; machineCodeIndex++;
            continue;
        }
        if (strcmp(opcode, "jmp") == 0) {
            machineCode[machineCodeIndex] = 8; machineCodeIndex++;
            unsigned short address = findAddress(strtok(NULL, " "));
            machineCode[machineCodeIndex] = address; machineCodeIndex++;
            machineCode[machineCodeIndex] = address >> 8; machineCodeIndex++;
            continue;
        }
        if (strcmp(opcode, "jnz") == 0) {
            machineCode[machineCodeIndex] = 9; machineCodeIndex++;
            unsigned short address = findAddress(strtok(NULL, " "));
            machineCode[machineCodeIndex] = address; machineCodeIndex++;
            machineCode[machineCodeIndex] = address >> 8; machineCodeIndex++;
            continue;
        }
        if (strcmp(opcode, "label") == 0) {
            strcpy(labelTable[labelIndex].name, strtok(NULL, " "));
            labelTable[labelIndex].address = machineCodeIndex;
            labelIndex++;
            continue;
        }
        if (strcmp(opcode, "define") == 0) {
            strcpy(labelTable[labelIndex].name, strtok(NULL, " "));
            labelTable[labelIndex].address = findAddress(strtok(NULL, " "));
            labelIndex++;
            continue;
        }
        printf("WARNING: ignoring opcode %s\n", opcode);
    }
    if (saveFile("output.bin", machineCode, machineCodeIndex)) {
        printf("Failed to save binary file!\n");
        return 1;
    };
    return 0;
}