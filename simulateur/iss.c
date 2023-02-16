//TODO: and, or, xor j'ai pas l'impression que ça marche. Je pense il faudrait les convertires en binaires
//      parce que 0010 (2) | 0110 (6) = 8 --> 1000 (8) et pas 0110 (6), il ne retient pas la retenu en faite
//TODO: valeur signé
//TODO: gestion de la mem


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libgen.h>

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

#define NB_REGS 32

uint32_t regs[NB_REGS];

char **instrs = NULL;
int instr_num = 0;

int running = 1;

typedef struct {
    int32_t opcode;
    int32_t imm;
    int32_t r_beta;
    int32_t r_alpha;
    int32_t o;
    int32_t r;
    int32_t a;
    int32_t n;
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
    char instrNum[11] = "";
    char instr[11] = "";

    int i = 0;
    while (fscanf(ptr, "%s %s", instrNum, instr) != EOF) {
        instrs = (char**)realloc(instrs, sizeof(instrs) * (i+1));
        instrs[i] = (char*)malloc(strlen(instr));
        strcpy(instrs[i], instr);
        i++;
    }
    fclose(ptr);

    return instrs;
}

void showRegs(){
    printf("regs = ");
    for (int i = 0; i < NB_REGS; i++){
        printf("0x%04X ", (regs[i] & 0x0000ffff));
    }
    printf("\n");
}

uint32_t fetch(){
    char* instr = instrs[instr_num++];
    return (uint32_t)strtol(instr, NULL, 0);
}

Instr_t decode(uint32_t instr){
    Instr_t my_instr;
    uint32_t opcode = (instr >> 27) & 0x0000001f;
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
            my_instr.a       = (instr)       & 0x001fffff;
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
    uint32_t o;
    
    if (instr.imm) 
        o = instr.o;
    else
         o = regs[instr.o];

    switch (instr.opcode){
        case STOP:
            running = 0;
            printf("(stop)\n");
            break;
        case ADD:
            printf("add r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            regs[instr.r_beta] = regs[instr.r_alpha] + o;
            break;
        case SUB:
            printf("sub r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            regs[instr.r_beta] = regs[instr.r_alpha] - o;
            break;
        case MUL:
            printf("mul r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            regs[instr.r_beta] = regs[instr.r_alpha] * o;
            break;
        case DIV:
            printf("div r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            regs[instr.r_beta] = regs[instr.r_alpha] / o;
            break;
        case AND:
            printf("and r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            regs[instr.r_beta] = regs[instr.r_alpha] & o;
            break;
        case OR:
            printf("or r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            regs[instr.r_beta] = (regs[instr.r_alpha] | o) + (regs[instr.r_alpha] & o);
            break;
        case XOR:
            printf("xor r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            regs[instr.r_beta] = (regs[instr.r_alpha] ^ o) + (regs[instr.r_alpha] & o);
            break;
        case SHL:
            printf("shl r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            regs[instr.r_beta] = regs[instr.r_alpha] << o;
            break;
        case SHR:
            printf("shr r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            regs[instr.r_beta] = regs[instr.r_alpha] >> o;
            break;
        case SLT:
            printf("slt r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            if (regs[instr.r_alpha] < o) 
                regs[instr.r_beta] = 1;
            else 
                regs[instr.r_beta] = 0;
            break;
        case SLE:
            printf("sle r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            if (regs[instr.r_alpha] <= o) 
                regs[instr.r_beta] = 1;
            else 
                regs[instr.r_beta] = 0;
            break;
        case SEQ:
            printf("seq r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            if (regs[instr.r_alpha] == o) 
                regs[instr.r_beta] = 1;
            else 
                regs[instr.r_beta] = 0;
            break;
        case LOAD:
            printf("load r%d %d r%d\n", instr.r_alpha, o, instr.r_beta);
            //TODO    
            break;
        case STORE:
            printf("store r%d %d r%d\n", instr.r_alpha, o, instr.r_beta);
            //TODO    
            break;
        case JMP:
            printf("jmp %d r%d\n", instr.o, instr.r);
            regs[instr.r] = instr_num + 1;
            instr_num = instr.o;
            break;
        case BRAZ:
            printf("braz r%d, %d\n", instr.r, instr.a);
            if (regs[instr.r] == 0) 
                instr_num = instr.a;
            break;
        case BRANZ:
            printf("branz r%d, %d\n", instr.r, instr.a);
            if (regs[instr.r] != 0) 
                instr_num = instr.a;
            break;
        case SCALL:
            printf("(%d, %d)\n", instr.opcode, instr.a);
            //TODO    
            break;
        default:
            printf("(%d, %d, %d, %d, %d)\n", instr.opcode, instr.r_alpha, instr.imm, instr.o, instr.r_beta);
            break;
    }

    regs[0] = 0;
}

int main(int argc, const char* argv[]){
    FILE* ptrFile;
    char* filePath;

    filePath = strcat(dirname(realpath(__FILE__, NULL)), "/../data.bin");
    ptrFile = openFile(filePath);
    instrs = parseFile(ptrFile);

    while(running){
        showRegs();
        uint32_t instr = fetch();
        Instr_t instr_decode = decode(instr);
        eval(instr_decode);
    }

    return 0;
}