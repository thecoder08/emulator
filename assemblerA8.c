#include <stdio.h>
#include "loadfile.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[100];
    unsigned char address;
} Label;

unsigned char code[10000];
unsigned char codeCopy[10000];
unsigned char machineCode[0x10000];
unsigned char machineCodeIndex = 0;
Label labelTable[100];
int labelIndex = 0;

unsigned char findAddress(char* token) {
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
    // load source code
    if(loadFile(argv[1], code)) {
        printf("Failed to open code file \"%s\"!\n", argv[1]);
        return 1;
    }
    // make copy of code for second pass
    strcpy(codeCopy, code);
    // first pass: labels and defines
    char* savedLine = NULL;
    char* line = strtok_r(code, "\n", &savedLine);
    for (; line != NULL; line = strtok_r(NULL, "\n", &savedLine)) {
        char* opcode = strtok(line, " ");
        if (strcmp(opcode, "nop") == 0) {
            machineCodeIndex += 1;
            continue;
        }
        if (strcmp(opcode, "load") == 0) {
            machineCodeIndex += 2;
            continue;
        }
        if (strcmp(opcode, "add") == 0) {
            machineCodeIndex += 2;
            continue;
        }
        if (strcmp(opcode, "sub") == 0) {
            machineCodeIndex += 2;
            continue;
        }
        if (strcmp(opcode, "store") == 0) {
            machineCodeIndex += 2;
            continue;
        }
        if (strcmp(opcode, "jmp") == 0) {
            machineCodeIndex += 2;
            continue;
        }
        if (strcmp(opcode, "jnz") == 0) {
            machineCodeIndex += 2;
            continue;
        }
        if (strcmp(opcode, "label") == 0) {
            strcpy(labelTable[labelIndex].name, strtok(NULL, " "));
            char* offset = strtok(NULL, " ");
            if (offset == NULL) {
              labelTable[labelIndex].address = machineCodeIndex;
            }
            else {
              labelTable[labelIndex].address = machineCodeIndex + (unsigned char)strtol(offset, NULL, 0);
            }
            labelIndex++;
            continue;
        }
        if (strcmp(opcode, "define") == 0) {
            strcpy(labelTable[labelIndex].name, strtok(NULL, " "));
            labelTable[labelIndex].address = findAddress(strtok(NULL, " "));
            labelIndex++;
            continue;
        }
    }
    // second pass: instructions
    machineCodeIndex = 0;
    savedLine = NULL;
    line = strtok_r(codeCopy, "\n", &savedLine);
    for (; line != NULL; line = strtok_r(NULL, "\n", &savedLine)) {
        char* opcode = strtok(line, " ");
        if (strcmp(opcode, "nop") == 0) {
            machineCode[machineCodeIndex] = 0; machineCodeIndex++;
            continue;
        }
        if (strcmp(opcode, "load") == 0) {
            // immidiate
            char* token2 = strtok(NULL, " ");
            if (strcmp(token2, "#") == 0) {
                machineCode[machineCodeIndex] = 1; machineCodeIndex++;
                machineCode[machineCodeIndex] = findAddress(strtok(NULL, " ")); machineCodeIndex++;
            }
            // memory
            else {
                machineCode[machineCodeIndex] = 2; machineCodeIndex++;
                machineCode[machineCodeIndex] = findAddress(token2); machineCodeIndex++;
            }
            continue;
        }
        if (strcmp(opcode, "add") == 0) {
            // immidiate
            char* token2 = strtok(NULL, " ");
            if (strcmp(token2, "#") == 0) {
                machineCode[machineCodeIndex] = 3; machineCodeIndex++;
                machineCode[machineCodeIndex] = findAddress(strtok(NULL, " ")); machineCodeIndex++;
            }
            // memory
            else {
                machineCode[machineCodeIndex] = 4; machineCodeIndex++;
                machineCode[machineCodeIndex] = findAddress(token2); machineCodeIndex++;
            }
            continue;
        }
        if (strcmp(opcode, "sub") == 0) {
            // immidiate
            char* token2 = strtok(NULL, " ");
            if (strcmp(token2, "#") == 0) {
                machineCode[machineCodeIndex] = 5; machineCodeIndex++;
                machineCode[machineCodeIndex] = findAddress(strtok(NULL, " ")); machineCodeIndex++;
            }
            // memory
            else {
                machineCode[machineCodeIndex] = 6; machineCodeIndex++;
                machineCode[machineCodeIndex] = findAddress(token2); machineCodeIndex++;
            }
            continue;
        }
        if (strcmp(opcode, "store") == 0) {
            machineCode[machineCodeIndex] = 7; machineCodeIndex++;
            machineCode[machineCodeIndex] = findAddress(strtok(NULL, " ")); machineCodeIndex++;
            continue;
        }
        if (strcmp(opcode, "jmp") == 0) {
            machineCode[machineCodeIndex] = 8; machineCodeIndex++;
            machineCode[machineCodeIndex] = findAddress(strtok(NULL, " ")); machineCodeIndex++;
            continue;
        }
        if (strcmp(opcode, "jnz") == 0) {
            machineCode[machineCodeIndex] = 9; machineCodeIndex++;
            machineCode[machineCodeIndex] = findAddress(strtok(NULL, " ")); machineCodeIndex++;
            continue;
        }
        // we still have these to avoid a warning
        if (strcmp(opcode, "label") == 0) {
            continue;
        }
        if (strcmp(opcode, "define") == 0) {
            continue;
        }
        printf("WARNING: ignoring opcode %s\n", opcode);
    }
    // save output file
    if (saveFile("outputA8.bin", machineCode, machineCodeIndex)) {
        printf("Failed to save binary file!\n");
        return 1;
    };
    return 0;
}
