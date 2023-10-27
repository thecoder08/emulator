#include <stdio.h>
#include "loadfile.h"
#include <xgfx/window.h>
#include <xgfx/drawing.h>

unsigned char memory[0x10000];
#define EVENT_BUFFER_SIZE 100
XEvent eventBuffer[EVENT_BUFFER_SIZE];

void updateFramebuffer() {
    for (unsigned short y = 0; y < 192; y++) {
            for (unsigned short x = 0; x < 256; x++) {
                unsigned short address = (y * 256) + x + 0x4000;
                unsigned char red = memory[address] & 0b11000000;
                unsigned char green = memory[address] & 0b00111000;
                unsigned char blue = memory[address] & 0b00000111;
                rectangle(x * 4, y * 4, 4, 4, (red << 16) | (green << 11) | (blue << 5));
            }
        }
}

int main(int argc, char** argv) {
    unsigned char accumulator = 0;
    unsigned short programCounter = 0;
    if(loadFile(argv[1], memory)) {
        printf("Failed to open program file \"%s\"!\n", argv[1]);
        return 1;
    }
    initWindow(1024, 768, "Emulator");
    for (int pulse = 0; 1; pulse = (pulse < 100000 ? pulse + 1 : 0)) {
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
            accumulator = memory[memory[programCounter] | (memory[programCounter + 1] << 8)];
            programCounter += 2;
            break;

            // add immediate
            case 3:
            accumulator += memory[programCounter];
            programCounter++;
            break;

            // add from memory
            case 4:
            accumulator += memory[memory[programCounter] | (memory[programCounter + 1] << 8)];
            programCounter += 2;
            break;

            // subtract immediate
            case 5:
            accumulator -= memory[programCounter];
            programCounter++;
            break;

            // subtract from memory
            case 6:
            accumulator -= memory[memory[programCounter] | (memory[programCounter + 1] << 8)];
            programCounter += 2;
            break;

            // store
            case 7:
            memory[memory[programCounter] | (memory[programCounter + 1] << 8)] = accumulator;
            programCounter += 2;
            break;

            // unconditional jump
            case 8:
            programCounter = memory[programCounter] | (memory[programCounter + 1] << 8);
            break;

            // jump if not zero
            case 9:
            if (accumulator != 0) {
                programCounter = memory[programCounter] | (memory[programCounter + 1] << 8);
            }
            else {
                programCounter += 2;
            }
            break;
        }
        // update graphics every 500 clock cycles
        if (pulse == 0) {
            updateFramebuffer();
            updateWindow();
        }
    }
}
