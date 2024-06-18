# Emulator
An emulator for my own 8-bit computer and processor architecture. It's a bit unwieldly to use because indexed addressing is done by overwriting code. So be careful!

This repository includes an assembler and emulator.
## Compiling
To compile the assembler, run:
```
gcc assembler.c loadfile.c -o assembler
```
To compile the emulator, run:
```
gcc emulator.c loadfile.c -lxgfx -o emulator
```
The emulator requires [libxgfx](https://github.com/thecoder08/xgfx) to display graphics.
## Instruction set
If an instruction takes an address as an operand, the first byte should be the high byte, followed by the low byte. This makes it a little-endian architecture. The opcodes of all instructions for the processor are as follows:
```
0x00 - No operation
0x01 - Load immidiate: takes one byte operand: the number to load into the accumulator.
0x02 - Load memory: takes two byte operand: the address from which to load the accumulator.
0x03 - Add immidiate: takes one byte operand: the number to add to the accumulator.
0x04 - Add memory: takes two byte operand: the address from which to add to the accumulator.
0x05 - Subtract immidiate: takes one byte operand: the number to subtract from the accumulator.
0x06 - Subtract memory: takes two byte operand: the address from which to subtract from the accumulator.
0x07 - Store: takes two byte operand: the address to which the accumulator will be stored.
0x08 - Unconditional jump: takes two byte operand: the address to which to jump.
0x09 - Conditional jump: takes two byte operand: the address to which to jump IF the accumulator does not equal zero.
0x0A - Jump with link: takes two byte operand: the address to which to jump. Also sets the link register with the address of the next instruction.
0x0B - Return to link: takes no operand. Jumps to the address stored in the link register.
```
Any other instruction opcodes have no operation.
## Assembler syntax
An assembly instruction in my assembler is written as follows:
```
operation operand\n
```
OR
```
operation # immidiate\n
```
That is, the name of the operation, followed by a space, followed optionally by a hash symbol and space if the instruction is an immidiate, followed by the operand, followed by a newline (`\n`).
### Operations
The operations are as follows:
```
nop
load
add
sub
store
jmp
jnz
jl
rl
```
Each operation translates to a machine code instruction. The opcode of load, add, and sub instructions depends on whether the operation is followed by a hash symbol (immidiate value). Any operation not in this list will be ignored by the assembler, allowing for comments.
### Operands
Each operand may be a number parsable by the C `strtol` function. It should be no greater that the largest possible integer for the type of operand requested, e.g. 65535 (0xFFFF) for 16-bit operands, or 255 (0xFF) for 8-bit operands. It may also be a symbol, defined by the `define` or `label` commands. Any extra operands for an operation will be ignored by the assembler.
### Symbols
Symbols can be defined by the `define` or `label` commands. These commands do not translate into machine code instructions.

Label uses the following syntax:
```
label symbol\n
```
OR
```
label symbol offset\n
```
This sets the symbol with the specified name to the machine code address where this label appears in code. If an offset is added, the symbol is set to the address plus the offset. This can be used to update the address for a read or write in memory.

Define uses the following syntax:
```
define symbol value\n
```
The sets the symbol with the specified name to that specified value. This can be used to allocate memory for variables.

For a basic example that displays an interesting pattern, check out `videotest.asm`.
### Usage
To assemble a program, run:
```
./assembler [source-file]
```
This will produce a binary program file called `output.bin` that can be executed by the emulator.
## Emulator
By calling the emulator with the compiled binary program, the program will be loaded into the start of memory. The computer's memory map is essentially all RAM; memory from 0x4000 to 0xFFFF is used as the framebuffer. The emulator has a 256x192 graphics mode with 256 colors. Each byte therefore represents one pixel. All other memory can be used for code or data. When the emulator starts, the program counter is set to 0x0000, which means your program will start executing at the start of memory. To load a program and start execution, run:
```
./emulator [program-file]
```
This should open a window that displays the contents of the framebuffer. By closing the window, the emulator stops.
### Update
An additional parameter can be passed to the emulator that will enable the experimental 8-bit address bus with basic 8x8 black and white graphics. (It doesn't matter what this parameter is.) This is to allow testing for my future Minecraft redstone computer. To test it, assemble and run the `a8test.asm` script using the `assemblerA8.c` assembler.