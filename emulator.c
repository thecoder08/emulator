#include <stdio.h>
#include "loadfile.h"
#include <xgfx/window.h>
#include <xgfx/drawing.h>

unsigned char memory[0x10000];
#define EVENT_BUFFER_SIZE 100
XEvent eventBuffer[EVENT_BUFFER_SIZE];
unsigned char a8mode = 0;

void updateFramebuffer() {
    if (a8mode) {
        for (unsigned char y = 0; y < 8; y++) {
            for (unsigned char x = 0; x < 8; x++) {
                unsigned char address = 0xf8 + y;
                if (memory[address] & (1 << 7-x)) {
                    rectangle(x * 96, y * 96, 96, 96, 0x00ffffff);
                }
                else {
                    rectangle(x * 96, y * 96, 96, 96, 0x00000000);
                }
            }
        }
    }
    else {
        for (unsigned short y = 0; y < 192; y++) {
            for (unsigned short x = 0; x < 256; x++) {
                unsigned short address = (y * 256) + x + 0x4000;
                unsigned char red = memory[address] & 0b11000000;
                unsigned char green = (memory[address] & 0b00111000) << 2;
                unsigned char blue = (memory[address] & 0b00000111) << 5;
                rectangle(x * 4, y * 4, 4, 4, (red << 16) | (green << 8) | blue);
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc > 2) {
        a8mode = 1;
    }
    unsigned char accumulator = 0;
    unsigned short programCounter = 0;
    if(loadFile(argv[1], memory)) {
        printf("Failed to open program file \"%s\"!\n", argv[1]);
        return 1;
    }
    if (a8mode) {
        initWindow(768, 768, "Emulator (A8 mode)");
    }
    else {
        initWindow(1024, 768, "Emulator (A16 mode)");
        if(loadFile("lenna-a16.data", memory + 0x4000)) {
            printf("Failed to load Lenna startup image\n");
        }
    }
    for (int pulse = 0; 1; pulse = (pulse < 10000 ? pulse + 1 : 0)) {
        int eventsRead = checkWindowEvents(eventBuffer, EVENT_BUFFER_SIZE);
        for (int i = 0; i < eventsRead; i++) {
            XEvent event = eventBuffer[i];
            if (event.type == ClosedWindow) {
                return 0;
            }
        }
        unsigned char instruction = memory[programCounter];
        programCounter++;
        switch(instruction) {
            // load immediate
            case 1:
            accumulator = memory[programCounter];
            programCounter++;
            break;

            // load from memory
            case 2:
            if (a8mode) {
                accumulator = memory[memory[programCounter]];
                programCounter++;
            }
            else {
                accumulator = memory[memory[programCounter] | (memory[programCounter + 1] << 8)];
                programCounter += 2;
            }
            break;

            // add immediate
            case 3:
            accumulator += memory[programCounter];
            programCounter++;
            break;

            // add from memory
            case 4:
            if (a8mode) {
                accumulator += memory[memory[programCounter]];
                programCounter++;
            }
            else {
                accumulator += memory[memory[programCounter] | (memory[programCounter + 1] << 8)];
                programCounter += 2;
            }
            break;

            // subtract immediate
            case 5:
            accumulator -= memory[programCounter];
            programCounter++;
            break;

            // subtract from memory
            case 6:
            if (a8mode) {
                accumulator -= memory[memory[programCounter]];
                programCounter++;
            }
            else {
                accumulator -= memory[memory[programCounter] | (memory[programCounter + 1] << 8)];
                programCounter += 2;
            }
            break;

            // store
            case 7:
            if (a8mode) {
                memory[memory[programCounter]] = accumulator;
                programCounter++;
            }
            else {
                memory[memory[programCounter] | (memory[programCounter + 1] << 8)] = accumulator;
                programCounter += 2;
            }
            break;

            // unconditional jump
            case 8:
            if (a8mode) {
                programCounter = memory[programCounter];
            }
            else {
                programCounter = memory[programCounter] | (memory[programCounter + 1] << 8);
            }
            break;

            // jump if not zero
            case 9:
            if (a8mode) {
                if (accumulator != 0) {
                    programCounter = memory[programCounter];
                }
                else {
                    programCounter++;
                }
            }
            else {
                if (accumulator != 0) {
                    programCounter = memory[programCounter] | (memory[programCounter + 1] << 8);
                }
                else {
                    programCounter += 2;
                }
            }
            break;
        }
        // update graphics every 10000 instructions
        if (pulse == 0) {
            updateFramebuffer();
            updateWindow();
        }
    }
}
