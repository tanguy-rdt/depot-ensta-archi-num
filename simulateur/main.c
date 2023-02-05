#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STOP   0
#define ADD    1
#define SUB    2
#define MUL    3
#define DIV    4
#define AND    5
#define OR     6
#define XOR    7
#define SHL    8
#define SHR    9
#define SLT    10
#define SLE    11
#define SEQ    12
#define LOAD   13
#define STORE  14
#define JMP    15
#define BRAZ   16
#define BRANZ  17
#define SCALL  18

#define R0     0
#define R1     1
#define R2     2
#define R3     3
#define R4     4
#define R5     5
#define R6     6
#define R7     7
#define R8     8
#define R9     9

#define NB_REGS 9

unsigned regs[ NB_REGS ];

char **instrs = NULL;
int instr_num = 0;

int running = 1;

typedef struct {
    int opcode;
    int imm;
    int r_beta;
    int r_alpha;
    int o;
    int r;
    int a;
    int n;
} Instr_t;

FILE* openFile(char *pathFile){
    FILE* ptr;
    ptr = fopen(pathFile, "r");
    
    if (ptr == NULL){
        printf("Unable to read the file");
    }
    else {
        return ptr;
    }

    return NULL;
}

char** parseFile(FILE* ptr){
    char instr_num[11] = "";
    char instr[11] = "";

    instrs = (char**)malloc(19 * sizeof(char*));

    int i = 0;
    while (fscanf(ptr, "%s %s", instr_num, instr) != EOF) {
        //instrs = (char**)malloc(sizeof(char*));
        instrs[i] = (char*)malloc (strlen (instr));
        strcpy (instrs[i], instr);
        i++;
    }

    fclose(ptr);

    return instrs;
}

void showRegs(){
    printf("regs = ");
    for (int i = 0; i < NB_REGS; i++){
        printf("%04X ", regs[i]);
    }
    printf("\n");
}

int32_t fetch(){
    char* instr = instrs[instr_num++];
    return (int)strtol(instr, NULL, 0);
}

Instr_t decode(int instr){
    Instr_t my_instr;
    int opcode = (instr >> 27) & 0x0000001f;
    my_instr.opcode = opcode;

    switch(opcode){
        case STOP:
            break;
        case JMP:
            my_instr.imm     = (instr >> 26) & 0x00000001;
            my_instr.o       = (instr >> 5)  & 0x000fffff;
            my_instr.r       = (instr)       & 0x0000001f;
            break;
        case BRAZ:
            my_instr.r       = (instr >> 22) & 0x0000001f;
            my_instr.a       = (instr)       & 0x001fffff;
            break;
        case BRANZ: 
            my_instr.r       = (instr >> 22) & 0x0000001f;
            my_instr.a       = (instr)       & 0x03ffffff;
            break;
        case SCALL:
            my_instr.n       = (instr)       & 0x001fffff;
            break;
        default:
            my_instr.r_alpha = (instr >> 22) & 0x0000001f;
            my_instr.imm     = (instr >> 21) & 0x00000001;
            my_instr.o       = (instr >> 5)  & 0x0000ffff;
            my_instr.r_beta  = (instr)       & 0x0000001f;
            break;
    }

    return my_instr;
}

void eval(Instr_t instr){
    switch (instr.opcode){
        case STOP:
            running = 0;
            printf("(stop)\n");
            break;
        case JMP:
            printf("(%d, %d, %d, %d)\n", instr.opcode, instr.imm, instr.o, instr.r);
            break;
        case BRAZ:
            printf("(%d, %d, %d)\n", instr.opcode, instr.r, instr.a);
            break;

        case BRANZ:
            printf("(%d, %d, %d)\n", instr.opcode, instr.r, instr.a);
            break;
        case SCALL:
            printf("(%d, %d)\n", instr.opcode, instr.a);
            break;
        default:
            printf("(%d, %d, %d, %d, %d)\n", instr.opcode, instr.r_alpha, instr.imm, instr.o, instr.r_beta);
            break;
    }

}

int main(int argc, const char* argv[]){
    FILE* ptrFile;

    ptrFile = openFile("data.bin");
    instrs = parseFile(ptrFile);

    while(running){
        showRegs();
        int32_t instr = fetch();
        Instr_t instr_decode = decode(instr);
        eval(instr_decode);
    }

    return 0;
}