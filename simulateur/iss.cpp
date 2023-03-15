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
#include <stdint.h>
#include <unistd.h>
#include <time.h>

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

#define PROGRAM_MEMORY 2048
#define INSTR_NB_OCTET 4
#define INSTR_MAX (PROGRAM_MEMORY/INSTR_NB_OCTET)-1

Cache cache;

typedef struct {
    int running;
    int regs[NB_REGS];
    int pc;
    long instrCnt;
    long cycleCnt;
} Cpu_t;

Cpu_t cpu = {1, {}};

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

Instr_t instrDecode = {};

int instrs[INSTR_MAX];


void error(int errorCode, FILE* msg){
    printf("%s\n", msg);
    printf("Exit code: %d\n", errorCode);
    exit(errorCode);
}

FILE* openFile(char *pathFile){
    FILE* ptr;
    ptr = fopen(pathFile, "r");

    if (ptr == NULL){
        fprintf(stderr, "ERROR: Unable to open the file: %s", pathFile);
        error(2, stderr);
    }
    else {
        return ptr;
    }

    return NULL;
}

void parseFile(FILE* ptr){
    char addr[11] = "";
    char instr[11] = "";

    int i = 0;
    while (fscanf(ptr, "%s %s", addr, instr) != EOF) {
        if (i > INSTR_MAX){
            fprintf(stderr, "ERROR: Memory program filled");
            error(3, stderr);
        }
        else {
            instrs[i] = (int)strtol(instr, NULL, 0);
            i++;
        }
    }
    fclose(ptr);
}

int fetch(){
    cpu.instrCnt++;
    return instrs[cpu.pc++];
}

void decode(int instr){
    instrDecode.opcode = (instr >> 27) & 0x0000001f;

    switch(instrDecode.opcode){
        case STOP:
            break;
        case JMP:
            instrDecode.imm     = (instr >> 26) & 0x00000001;
            instrDecode.o       = (instr >> 5)  & 0x0000ffff;
            instrDecode.r       = (instr)       & 0x0000001f;
            break;
        case BRAZ:
            instrDecode.r       = (instr >> 22) & 0x0000001f;
            instrDecode.a       = (instr)       & 0x001fffff;
            break;
        case BRANZ: 
            instrDecode.r       = (instr >> 22) & 0x0000001f;
            instrDecode.a       = (instr)       & 0x001fffff;
            break;
        case SCALL:
            instrDecode.n       = (instr)       & 0x001fffff;
            break;
        default:
            instrDecode.rAlpha  = (instr >> 22) & 0x0000001f;
            instrDecode.imm     = (instr >> 21) & 0x00000001;
            instrDecode.o       = (int16_t)((instr >> 5)  & 0x0000ffff);
            instrDecode.rBeta   = (instr)       & 0x0000001f;
            break;
    }
}

void showInstr(){
    char* instrStrMask;

    if (instrDecode.imm){
        if (instrDecode.opcode == JMP)
            instrStrMask = "%s r%d r%d";
        else
            instrStrMask = "%s r%d %d r%d";
    }
    else{
        if (instrDecode.opcode == BRAZ || instrDecode.opcode == BRANZ || instrDecode.opcode == JMP)
            instrStrMask = "%s r%d %d";
        else if (instrDecode.opcode == SCALL)
            instrStrMask = "%s %d";
        else if (instrDecode.opcode == STOP)
            instrStrMask = "%s";
        else
            instrStrMask = "%s r%d r%d r%d";
    }

    switch (instrDecode.opcode){
        case STOP:  fprintf(stdout, instrStrMask, "stop");                                                        break;
        case ADD:   fprintf(stdout, instrStrMask, "add",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case SUB:   fprintf(stdout, instrStrMask, "sub",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case MUL:   fprintf(stdout, instrStrMask, "mul",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case DIV:   fprintf(stdout, instrStrMask, "div",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case AND:   fprintf(stdout, instrStrMask, "and",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case OR:    fprintf(stdout, instrStrMask, "or",    instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case XOR:   fprintf(stdout, instrStrMask, "xor",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case SHL:   fprintf(stdout, instrStrMask, "shl",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case SHR:   fprintf(stdout, instrStrMask, "shr",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case SLT:   fprintf(stdout, instrStrMask, "slt",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case SLE:   fprintf(stdout, instrStrMask, "sle",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case SEQ:   fprintf(stdout, instrStrMask, "seq",   instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case LOAD:  fprintf(stdout, instrStrMask, "load",  instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case STORE: fprintf(stdout, instrStrMask, "store", instrDecode.rAlpha, instrDecode.o, instrDecode.rBeta); break;
        case JMP:   fprintf(stdout, instrStrMask, "jmp",   instrDecode.o,      instrDecode.r);                    break;
        case BRAZ:  fprintf(stdout, instrStrMask, "braz",  instrDecode.r,      instrDecode.a);                    break;
        case BRANZ: fprintf(stdout, instrStrMask, "branz", instrDecode.r,      instrDecode.a);                    break;
        case SCALL: fprintf(stdout, instrStrMask, "scall", instrDecode.n);                                        break;
    }

    printf("%s\n", stdout);
}

void overflow(long val){
     if ((val > (signed)0x7fffffff) || (val < (signed)0x80000000)){
        fprintf(stderr, "ERROR: overflow");
        error(4, stderr);
    }
}

void eval(){
    int o;
    
    if (instrDecode.imm) 
        o = instrDecode.o;
    else
         o = cpu.regs[instrDecode.o];

    switch (instrDecode.opcode){
        case STOP:
            cpu.running = 0;
            break;
        case ADD:
            overflow((long long)cpu.regs[instrDecode.rAlpha] + o);
            cpu.regs[instrDecode.rBeta] = cpu.regs[instrDecode.rAlpha] + o;
            cpu.cycleCnt++;
            break;
        case SUB:
            overflow((long long)cpu.regs[instrDecode.rAlpha] - o);
            cpu.regs[instrDecode.rBeta] = cpu.regs[instrDecode.rAlpha] - o;
            cpu.cycleCnt++;
            break;
        case MUL:
            overflow((long long)cpu.regs[instrDecode.rAlpha] * o);
            cpu.regs[instrDecode.rBeta] = cpu.regs[instrDecode.rAlpha] * o;
            cpu.cycleCnt += 2;
            break;
        case DIV:
            cpu.regs[instrDecode.rBeta] = cpu.regs[instrDecode.rAlpha] / o;
            cpu.cycleCnt += 2;
            break;
        case AND:
            cpu.regs[instrDecode.rBeta] = cpu.regs[instrDecode.rAlpha] & o;
            cpu.cycleCnt++;
            break;
        case OR:
            cpu.regs[instrDecode.rBeta] = (cpu.regs[instrDecode.rAlpha] | o) + (cpu.regs[instrDecode.rAlpha] & o);
            cpu.cycleCnt++;
            break;
        case XOR:
            cpu.regs[instrDecode.rBeta] = (cpu.regs[instrDecode.rAlpha] ^ o);
            cpu.cycleCnt++;
            break;
        case SHL:
            cpu.regs[instrDecode.rBeta] = cpu.regs[instrDecode.rAlpha] << o;
            cpu.cycleCnt++;
            break;
        case SHR:
            cpu.regs[instrDecode.rBeta] = cpu.regs[instrDecode.rAlpha] >> o;
            cpu.cycleCnt++;
            break;
        case SLT:
            if (cpu.regs[instrDecode.rAlpha] < o) 
                cpu.regs[instrDecode.rBeta] = 1;
            else 
                cpu.regs[instrDecode.rBeta] = 0;
            cpu.cycleCnt++;
            break;
        case SLE:
            if (cpu.regs[instrDecode.rAlpha] <= o) 
                cpu.regs[instrDecode.rBeta] = 1;
            else 
                cpu.regs[instrDecode.rBeta] = 0;
            cpu.cycleCnt++;
            break;
        case SEQ:
            if (cpu.regs[instrDecode.rAlpha] == o) 
                cpu.regs[instrDecode.rBeta] = 1;
            else 
                cpu.regs[instrDecode.rBeta] = 0;
            cpu.cycleCnt++;
            break;
        case LOAD:
            cpu.regs[instrDecode.rBeta] = cache.read(cpu.regs[instrDecode.rAlpha] + instrDecode.o);
            if (cache.getCacheMiss())
                cpu.cycleCnt += 100;
            else
                cpu.cycleCnt += 2;
            break;
        case STORE:
            cache.write(cpu.regs[instrDecode.rAlpha] + instrDecode.o, cpu.regs[instrDecode.rBeta]);
            cpu.cycleCnt += 100;  
            break;
        case JMP:
            cpu.regs[instrDecode.r] = cpu.pc;
            cpu.pc = o;
            cpu.cycleCnt += 2;
            break;
        case BRAZ:
            if (cpu.regs[instrDecode.r] == 0) 
                cpu.pc = instrDecode.a;
            cpu.cycleCnt += 2;
            break;
        case BRANZ:
            if (cpu.regs[instrDecode.r] != 0) 
                cpu.pc = instrDecode.a;
            cpu.cycleCnt += 2;
            break;
        case SCALL:
            if (instrDecode.n == 0){
                int val = 0;
                scanf("%d", &val);
                cpu.regs[1] = val;
            }
            else if (instrDecode.n == 1){
                printf("%d", cpu.regs[1]);
            }
            else if (instrDecode.n == 2){
                int pidScreen = fork();
                if (pidScreen == 0){
                    execlp("python3", "python3", strcat(dirname(realpath(__FILE__, NULL)), "/screen/screen.py"), NULL);
                }
                else if (pidScreen == -1){
                    fprintf(stderr, "ERROR: Failed to create the screen process");
                    error(4, stderr);
                }
            }
            else if (instrDecode.n == 3){
                cpu.regs[1] = getchar();
            }
            else if (instrDecode.n == 4){
                printf("%c", cpu.regs[1]);
            }
            else if (instrDecode.n == 5){
                srand(time(NULL));
                cpu.regs[1] = rand() % cpu.regs[2];
            }
    }

    cpu.regs[0] = 0;
}

void showCycle(){
    printf("Cycle number = %ld\n\n", cpu.cycleCnt);
}

void showRegs(){
    printf("regs = ");
    for (int i = 0; i < NB_REGS; i++){
        printf("%08x ", cpu.regs[i]);
    }
    printf("\n");
}

void showMips(double execTime){
    long mips = (cpu.instrCnt / (execTime))/1000000;
    printf("\n\nElapsed time %fs for %ld in %ld cycle\n", execTime, cpu.instrCnt, cpu.cycleCnt);
    printf("Estimated MIPS: %ld million par seconde", mips);
}


int main(int argc, const char* argv[]){
    clock_t start, end;

    FILE* ptrFile = NULL;
    char* filePath = NULL;

    if (argc != 2){
        printf("Usage:\n");
        printf("\t./iss <bin_file_path>\n\n");
        fprintf(stderr, "ERROR: You need to specify the input file");
        error(1, stderr);
    }
    else {
        filePath = realpath(argv[1], NULL);
    }

    ptrFile = openFile(filePath);
    parseFile(ptrFile);

    start = clock();
    while(cpu.running){
        int instr = fetch();
        decode(instr);

        #ifdef VERBOSE
            showInstr();
        #endif
    
        eval();

        #ifdef VERBOSE
            showRegs();
            showCycle();
        #endif
   
    }
    end = clock();

    double execTime = (double)(end-start)/CLOCKS_PER_SEC;
    showMips(execTime);

    return 0;
}