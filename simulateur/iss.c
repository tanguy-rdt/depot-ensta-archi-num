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
#include <unistd.h>
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
            my_instr.o       = (int16_t)((instr >> 5)  & 0x0000ffff);
            my_instr.rBeta   = (instr)       & 0x0000001f;
            break;
    }
    return my_instr;
}

void overflow(long val){
     if ((val > (signed)0x7fffffff) || (val < (signed)0x80000000)){
        fprintf(stderr, "ERROR: overflow");
        error(3, stderr);
    }
}


void showInstr(Instr_t instr){
    char* instrStrMask;

    if (instr.imm){
        if (instr.opcode == JMP)
            instrStrMask = "%s r%d r%d";
        else
            instrStrMask = "%s r%d %d r%d";
    }
    else{
        if (instr.opcode == BRAZ || instr.opcode == BRANZ || instr.opcode == JMP)
            instrStrMask = "%s r%d %d";
        else if (instr.opcode == SCALL)
            instrStrMask = "%s %d";
        else
            instrStrMask = "%s r%d r%d r%d";
    }

    switch (instr.opcode){
        case STOP:  fprintf(stdout, "%s", "stop");                                              break;
        case ADD:   fprintf(stdout, instrStrMask, "add", instr.rAlpha, instr.o, instr.rBeta);   break;
        case SUB:   fprintf(stdout, instrStrMask, "sub", instr.rAlpha, instr.o, instr.rBeta);   break;
        case MUL:   fprintf(stdout, instrStrMask, "mul", instr.rAlpha, instr.o, instr.rBeta);   break;
        case DIV:   fprintf(stdout, instrStrMask, "div", instr.rAlpha, instr.o, instr.rBeta);   break;
        case AND:   fprintf(stdout, instrStrMask, "and", instr.rAlpha, instr.o, instr.rBeta);   break;
        case OR:    fprintf(stdout, instrStrMask, "or", instr.rAlpha, instr.o, instr.rBeta);    break;
        case XOR:   fprintf(stdout, instrStrMask, "xor", instr.rAlpha, instr.o, instr.rBeta);   break;
        case SHL:   fprintf(stdout, instrStrMask, "shl", instr.rAlpha, instr.o, instr.rBeta);   break;
        case SHR:   fprintf(stdout, instrStrMask, "shr", instr.rAlpha, instr.o, instr.rBeta);   break;
        case SLT:   fprintf(stdout, instrStrMask, "slt", instr.rAlpha, instr.o, instr.rBeta);   break;
        case SLE:   fprintf(stdout, instrStrMask, "sle", instr.rAlpha, instr.o, instr.rBeta);   break;
        case SEQ:   fprintf(stdout, instrStrMask, "seq", instr.rAlpha, instr.o, instr.rBeta);   break;
        case LOAD:  fprintf(stdout, instrStrMask, "load", instr.rAlpha, instr.o, instr.rBeta);  break;
        case STORE: fprintf(stdout, instrStrMask, "store", instr.rAlpha, instr.o, instr.rBeta); break;
        case JMP:   fprintf(stdout, instrStrMask, "jmp", instr.o, instr.r);                     break;
        case BRAZ:  fprintf(stdout, instrStrMask, "braz", instr.r, instr.a);                    break;
        case BRANZ: fprintf(stdout, instrStrMask, "branz", instr.r, instr.a);                   break;
        case SCALL: fprintf(stdout, instrStrMask, "scall", instr.n);                            break;
    }

    printf("%s\n", stdout);
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
            break;
        case ADD:
            overflow((long long)regs[instr.rAlpha] + o);
            regs[instr.rBeta] = regs[instr.rAlpha] + o;
            cycleCnt += 1;
            break;
        case SUB:
            overflow((long long)regs[instr.rAlpha] - o);
            regs[instr.rBeta] = regs[instr.rAlpha] - o;
            cycleCnt += 1;
            break;
        case MUL:
            overflow((long long)regs[instr.rAlpha] * o);
            regs[instr.rBeta] = regs[instr.rAlpha] * o;
            cycleCnt += 2;
            break;
        case DIV:
            regs[instr.rBeta] = regs[instr.rAlpha] / o;
            cycleCnt += 2;
            break;
        case AND:
            regs[instr.rBeta] = regs[instr.rAlpha] & o;
            cycleCnt += 1;
            break;
        case OR:
            regs[instr.rBeta] = (regs[instr.rAlpha] | o) + (regs[instr.rAlpha] & o);
            cycleCnt += 1;
            break;
        case XOR:
            regs[instr.rBeta] = (regs[instr.rAlpha] ^ o) + (regs[instr.rAlpha] & o);
            cycleCnt += 1;
            break;
        case SHL:
            regs[instr.rBeta] = regs[instr.rAlpha] << o;
            cycleCnt += 1;
            break;
        case SHR:
            regs[instr.rBeta] = regs[instr.rAlpha] >> o;
            cycleCnt += 1;
            break;
        case SLT:
            if (regs[instr.rAlpha] < o) 
                regs[instr.rBeta] = 1;
            else 
                regs[instr.rBeta] = 0;
            cycleCnt += 1;
            break;
        case SLE:
            if (regs[instr.rAlpha] <= o) 
                regs[instr.rBeta] = 1;
            else 
                regs[instr.rBeta] = 0;
            cycleCnt += 1;
            break;
        case SEQ:
            if (regs[instr.rAlpha] == o) 
                regs[instr.rBeta] = 1;
            else 
                regs[instr.rBeta] = 0;
            cycleCnt += 1;
            break;
        case LOAD:
            regs[instr.rBeta] = cache.read(regs[instr.rAlpha] + instr.o);
            if (cache.getCacheMiss())
                cycleCnt += 100;
            else
                cycleCnt += 2;
            break;
        case STORE:
            cache.write(regs[instr.rAlpha] + instr.o, regs[instr.rBeta]);
            cycleCnt += 100;  
            break;
        case JMP:
            regs[instr.r] = instrNum;
            instrNum = o;
            cycleCnt += 1;
            break;
        case BRAZ:
            if (regs[instr.r] == 0) 
                instrNum = instr.a;
            cycleCnt += 1;
            break;
        case BRANZ:
            if (regs[instr.r] != 0) 
                instrNum = instr.a;
            cycleCnt += 1;
            break;
        case SCALL:
            if (instr.n == 0){
                int val = 0;
                scanf("%d", &val);
                regs[1] = val;
            }
            else if (instr.n == 1){
                printf("%d", regs[1]);
            }
            else if (instr.n == 2){
                int pidScreen = fork();
                if (pidScreen == 0){
                    execlp("python3", "python3", strcat(dirname(realpath(__FILE__, NULL)), "/screen/screen.py"), NULL);
                }
                else if (pidScreen == -1){
                    fprintf(stderr, "ERROR: Failed to create the screen process");
                    error(4, stderr);
                }
            }
            else if (instr.n == 3){
                printf("%c", regs[1]);
            }
            else if (instr.n == 4){
                srand(time(NULL));
                regs[1] = rand() % regs[2];
            }
    }

    regs[0] = 0;
}

void showMips(long execTime){
    printf("\n\n\nElapsed time %ldus for %d cycle\n", execTime, cycleCnt);
    long mips = (cycleCnt / (execTime * 0.000001));
    printf("Estimated MIPS: %d million par seconde", mips);
}


int main(int argc, const char* argv[]){
    struct timeval start, end;

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
    instrs = parseFile(ptrFile);

    gettimeofday(&start, 0);
    while(running){
        int instr = fetch();
        Instr_t instr_decode = decode(instr);

        #ifdef VERBOSE
            showInstr(instr_decode);
        #endif
    
        eval(instr_decode);

        #ifdef VERBOSE
            showRegs();
            showCycle();
        #endif
   
    }
    gettimeofday(&end, 0);

    long execTime = end.tv_usec - start.tv_usec;
    showMips(execTime);

    return 0;
}