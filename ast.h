typedef union {
    unsigned char immidiate;
    unsigned short address;
    unsigned char macro[100];
} Operand;

typedef struct {
    unsigned char name[10];
    unsigned char operandType; // 2 for macro, 1 for immidiate, 0 for address
    Operand operand;
} Instruction;

typedef struct {
    unsigned char name[100];
    unsigned short address;
} Macro;

typedef struct {
    Instruction instructions[1000];
    Macro macros[1000];
} Root;