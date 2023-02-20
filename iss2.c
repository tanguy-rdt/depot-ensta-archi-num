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

#define NB_REGS 32

int cycle_count = 0;

int16_t regs[ NB_REGS ];

char **instrs = NULL;
int instr_num = 0;

int running = 1;

typedef struct {
    int8_t opcode;
    int16_t imm;
    int16_t r_beta;
    int16_t r_alpha;
    int16_t o;
    int16_t r;
    int16_t a;
    int16_t n;
} Instr_t;

FILE* openFile(char *pathFile)
{
    FILE* ptr;
    ptr = fopen(pathFile, "r");
    
    if (ptr == NULL)
    {
        printf("Unable to read the file");
    }
    else 
    {
        return ptr;
    }

    return NULL;
}

char** parseFile(FILE* ptr){
    char instr_num[11] = "";
    char instr[11] = "";

    int i = 0;
    while (fscanf(ptr, "%s %s", instr_num, instr) != EOF) 
    {
        instrs = (char**)realloc(instrs, sizeof(instrs) * (i+1));
        instrs[i] = (char*)malloc (strlen (instr));
        strcpy (instrs[i], instr);
        i++;
    }

    fclose(ptr);

    return instrs;
}

void showRegs(){
    printf("regs = ");
    for (int i = 0; i < NB_REGS; i++)
    {
        printf("%04X ", regs[i]);
    }
    printf("\n");
}

void showCycle(){
    printf("Cycle number = %d\n", cycle_count);
}

int32_t fetch(){
    char* instr = instrs[instr_num++];
    return (int)strtol(instr, NULL, 0);
}

Instr_t decode(int instr)
{
    Instr_t my_instr;
    int opcode = (instr >> 27) & 0x0000001f;
    my_instr.opcode = opcode;

    switch(opcode){
        case STOP:
            break;
        case JMP:
            my_instr.imm     = (instr >> 26) & 0x00000001;
            my_instr.o       = (int16_t)(instr >> 5)  & 0x000fffff;
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
            my_instr.o       = (int16_t)((instr >> 5)  & 0x0000ffff);
            my_instr.r_beta  = (instr)       & 0x0000001f;
            break;
    }

    return my_instr;
}

void eval(Instr_t instr){
    uint32_t o;
    

    if (instr.imm) 
    {
        o = instr.o;
    }
    else 
    {
        o = regs[instr.o];
    }

    switch (instr.opcode){
        
        case ADD:
            regs[instr.r_beta] = regs[instr.r_alpha] + o;
            cycle_count += 1;
            if (instr.imm)
            {
                printf("add r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("add r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            
            break;

        case SUB:
            regs[instr.r_beta] = regs[instr.r_alpha] - o;
            cycle_count += 1;
            if (instr.imm)
            {
                printf("sub r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("sub r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            break;

        case DIV:
             if (regs[instr.r_alpha] & 0x80000000)
             {
                if(o & 0x80000000)
                {
                    regs[instr.r_alpha] = -regs[instr.r_alpha];
                    o = -o;
                    regs[instr.r_beta] = (regs[instr.r_alpha] / o);
                    regs[instr.r_alpha] = -regs[instr.r_alpha];
                    o = -o;
                }
                else
                {
                    regs[instr.r_alpha] = -regs[instr.r_alpha];
                    regs[instr.r_beta] = -(regs[instr.r_alpha] / o);
                    regs[instr.r_alpha] = -regs[instr.r_alpha];
                }
             }
             else if (o & 0x80000000)
             {
                o = -o;
                regs[instr.r_beta] = -(regs[instr.r_alpha] / o);
                o = -o;
             }
             else
             {
                regs[instr.r_beta] = (regs[instr.r_alpha] / o);
             }
             
             cycle_count += 1;

            printf("%d\n", regs[instr.r_beta]);

            
             if (instr.imm)
            {
                printf("div r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("div r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
             break;

        case MUL:
            regs[instr.r_beta] = regs[instr.r_alpha] * o;
            cycle_count += 1;
            if (instr.imm)
            {
                printf("mul r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("mul r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            break;

        case AND:
            regs[instr.r_beta] = regs[instr.r_alpha] & o;
            cycle_count += 1;
            if (instr.imm)
            {
                printf("and r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("and r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            break;

        case OR:
            regs[instr.r_beta] = regs[instr.r_alpha] | o;
            cycle_count += 1;
            if (instr.imm)
            {
                printf("or r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("or r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }   
            break;

        case XOR: 
            regs[instr.r_beta] = regs[instr.r_alpha] ^ o;
            cycle_count += 1;
            if (instr.imm)
            {
                printf("xor r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("xor r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }   
            break;

        case SHL:   
            regs[instr.r_beta] = regs[instr.r_alpha] << o;
            cycle_count += 1;
            if (instr.imm)
            {
                printf("shl r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("shl r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }   
            break;

        case SHR:
            regs[instr.r_beta] = regs[instr.r_alpha] >> o;
            cycle_count += 1;
            if (instr.imm)
            {
                printf("shr r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("shr r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            } 
            break;

        case SLT:
            if (regs[instr.r_alpha] < o)
            {
                regs[instr.r_beta] = 1;
            }    
            else 
            {
                regs[instr.r_beta] = 0;
            }
            cycle_count += 1;
            if (instr.imm)
            {
                printf("slt r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("slt r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            break;

        case SLE:
            if (regs[instr.r_alpha] <= o)
            {
                regs[instr.r_beta] = 1;
            }    
            else 
            {
                regs[instr.r_beta] = 0;
            }
            cycle_count += 1;
            if (instr.imm)
            {
                printf("sle r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("sle r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }    
            break;

        case SEQ:    
             if (regs[instr.r_alpha] == o)
            {
                regs[instr.r_beta] = 1;
            }    
            else 
            {
                regs[instr.r_beta] = 0;
            }
            cycle_count += 1;
            if (instr.imm)
            {
                printf("seq r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("seq r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            break;

        case LOAD:
            regs[instr.r_beta] = regs[instr.r_alpha+o];
            cycle_count += 100;
            if (instr.imm)
            {
                printf("load r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("load r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }   
            break;

        case STORE: 
            regs[instr.r_alpha + o] = regs[instr.r_beta];
            cycle_count += 100;
            if (instr.imm)
            {
                printf("store r%d %d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            else
            {
                printf("store r%d r%d r%d\n", instr.r_alpha, instr.o, instr.r_beta);
            }
            break;    

        case JMP:
            regs[instr.r] = instr_num + 1;
            instr_num = o;
            cycle_count += 100;
            printf("jmp %d r%d\n", instr.o, instr.r);
            break;

        case BRAZ:
            if (instr.r != 0)
            {
               instr_num = instr.a;
            }
            cycle_count += 100;
            printf("braz r%d, %d\n", instr.r, instr.a);
            break;

        case BRANZ:
            if (instr.r == 0)
            {
               instr_num = instr.a;
            }
            cycle_count += 100;
            printf("branz r%d, %d\n", instr.r, instr.a);
            break;

        case SCALL:
            //printf("(%d, %d)\n", instr.opcode, instr.a);
            break;

        case STOP:
            running = 0;
            printf("(stop)\n");
            break;
    }

}

int main(int argc, const char* argv[])
{
    FILE* ptrFile;

    ptrFile = openFile("data.bin");
    instrs = parseFile(ptrFile);

    while(running)
    {
        showRegs();
        showCycle();
        int32_t instr = fetch();
        Instr_t instr_decode = decode(instr);
        eval(instr_decode);
    }

    return 0;
}