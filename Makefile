all: assembler assemblerA8 emulator

emulator: loadfile.o emulator.o 3dproc.o
	gcc $^ -o $@ -lxgfx -lm

assembler: assembler.o loadfile.o
	gcc $^ -o $@

assemblerA8: assemblerA8.o loadfile.o
	gcc $^ -o $@

%.o: %.c
	gcc -c $^ -o $@

clean:
	rm -f *.o emulator assemblerA8 assembler