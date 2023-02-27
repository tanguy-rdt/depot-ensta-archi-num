/********************************************************
*                                                       *
*    PROJECT: ISS in C                                  *
*    ORGANIZATION: ENSTA Bretagne FIPASE 2024           *
*    AUTEUR: Tanguy ROUDAUT, Melvin DUBEE               *
*    DATE: 15/03/2023                                   *
*                                                       *
*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libgen.h>

#include <sys/time.h>

#include "cache.h"

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

Cache cache;

int regs[NB_REGS];

char **instrs = NULL;
int instrNum = 0;

int cycleCnt = 0;
int running = 1;

typedef struct {
    int opcode;
    int imm;
    int rBeta;
    int rAlpha;
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
    char addr[11] = "";
    char instr[11] = "";

    int i = 0;
    while (fscanf(ptr, "%s %s", addr, instr) != EOF) {
        instrs = (char**)realloc(instrs, sizeof(instrs) * (i+1));
        instrs[i] = (char*)malloc(strlen(instr));
        strcpy(instrs[i], instr);
        i++;
    }
    fclose(ptr);

    return instrs;
}



void showCycle(){
    printf("Cycle number = %d\n\n", cycleCnt);
}


void showRegs(){
    printf("regs = ");
    for (int i = 0; i < NB_REGS; i++){
        printf("%08x ", regs[i]);
    }
    printf("\n");
}


int fetch(){
    char* instr = instrs[instrNum++];
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
            my_instr.o       = (instr >> 5)  & 0x0000ffff;
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
            my_instr.rAlpha  = (instr >> 22) & 0x0000001f;
            my_instr.imm     = (instr >> 21) & 0x00000001;
            my_instr.o       = (int)((instr >> 5)  & 0x0000ffff);
            my_instr.rBeta   = (instr)       & 0x0000001f;
            break;
    }
    return my_instr;
}


void eval(Instr_t instr){
    int o;
    
    if (instr.imm) 
        o = instr.o;
    else
         o = regs[instr.o];

    switch (instr.opcode){
        case STOP:
            running = 0;
            printf("stop\n");
            break;
        case ADD:
            printf("add r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            regs[instr.rBeta] = regs[instr.rAlpha] + o;
            cycleCnt += 1;
            break;
        case SUB:
            printf("sub r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            regs[instr.rBeta] = regs[instr.rAlpha] - o;
            cycleCnt += 1;
            break;
        case MUL:
            printf("mul r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            regs[instr.rBeta] = regs[instr.rAlpha] * o;
            cycleCnt += 1;
            break;
        case DIV:
            printf("div r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            regs[instr.rBeta] = regs[instr.rAlpha] / o;
            cycleCnt += 1;
            break;
        case AND:
            printf("and r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            regs[instr.rBeta] = regs[instr.rAlpha] & o;
            cycleCnt += 1;
            break;
        case OR:
            printf("or r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            regs[instr.rBeta] = (regs[instr.rAlpha] | o) + (regs[instr.rAlpha] & o);
            cycleCnt += 1;
            break;
        case XOR:
            printf("xor r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            regs[instr.rBeta] = (regs[instr.rAlpha] ^ o) + (regs[instr.rAlpha] & o);
            cycleCnt += 1;
            break;
        case SHL:
            printf("shl r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            regs[instr.rBeta] = regs[instr.rAlpha] << o;
            cycleCnt += 1;
            break;
        case SHR:
            printf("shr r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            regs[instr.rBeta] = regs[instr.rAlpha] >> o;
            cycleCnt += 1;
            break;
        case SLT:
            printf("slt r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            if (regs[instr.rAlpha] < o) 
                regs[instr.rBeta] = 1;
            else 
                regs[instr.rBeta] = 0;
            cycleCnt += 1;
            break;
        case SLE:
            printf("sle r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            if (regs[instr.rAlpha] <= o) 
                regs[instr.rBeta] = 1;
            else 
                regs[instr.rBeta] = 0;
            cycleCnt += 1;
            break;
        case SEQ:
            printf("seq r%d %d r%d\n", instr.rAlpha, instr.o, instr.rBeta);
            if (regs[instr.rAlpha] == o) 
                regs[instr.rBeta] = 1;
            else 
                regs[instr.rBeta] = 0;
            cycleCnt += 1;
            break;
        case LOAD:
            printf("load r%d %d r%d\n", instr.rAlpha, o, instr.rBeta);
            regs[instr.rBeta] = cache.read(instr.rAlpha + instr.o);
            cycleCnt += 100;
            break;
        case STORE:
            printf("store r%d %d r%d\n", instr.rAlpha, o, instr.rBeta);
            cache.write(instr.rAlpha + instr.o, regs[instr.rBeta]);
            cycleCnt += 100;  
            break;
        case JMP:
            printf("jmp %d r%d\n", instr.o, instr.r);
            regs[instr.r] = instrNum + 1;
            instrNum = instr.o;
            cycleCnt += 100;
            break;
        case BRAZ:
            printf("braz r%d, %d\n", instr.r, instr.a);
            if (regs[instr.r] == 0) 
                instrNum = instr.a;
            cycleCnt += 1;
            break;
        case BRANZ:
            printf("branz r%d, %d\n", instr.r, instr.a);
            if (regs[instr.r] != 0) 
                instrNum = instr.a;
            cycleCnt += 1;
            break;
        case SCALL:
            printf("scall %d\n", instr.n);
            if (instr.n == 0){
                int val = 0;
                printf("Data to set: ");
                scanf("%d", &val);
                regs[1] = val;
            }
            else if (instr.n == 1){
                printf("Data in r1: %d\n", regs[1]);
            }
            else if (instr.n == 73){
                    printf("Comming soon...");
            }
    }

    regs[0] = 0;
}


int main(int argc, const char* argv[]){
    FILE* ptrFile;
    char* filePath;

    if (argc != 2){
        printf("Usage:\n");
        printf("\t./iss <bin_file_path>\n");
        exit(-1);
    }
    else {
        filePath = realpath(argv[1], NULL);
    }

    ptrFile = openFile(filePath);
    instrs = parseFile(ptrFile);

    while(running){
        showRegs();
        showCycle();
        int instr = fetch();
        Instr_t instr_decode = decode(instr);
        eval(instr_decode);
    }

    return 0;
}