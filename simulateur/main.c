# include <stdio.h>

# define NUM_REGS 7
unsigned regs[ NUM_REGS ];

#define ADD 1

int getOpcode(int arg){
    switch(arg){
        case 0x01:
            return ADD;
        default:
            break;
    }

    return -1;
}

int get_instr(int opcode, int reg1, int imm, int reg2, int reg3){
    int instr = 0;
    instr += opcode << 27;
    instr += reg1 << 22;
    instr += imm << 21;
    instr += reg2 << 5;
    instr += reg3;

    return instr;
}

int get_instr(int opcode, int reg1, int imm, int reg2){
    int instr = 0;
    instr += opcode << 27;
    instr += reg1 << 22;
    instr += imm << 21;
    instr += reg2 << 5;

    return instr;
}

int main(int argc, const char* argv[]){
    int instr = get_instr(ADD, 7, 0, 2, 3);

    printf("0x%x", instr);
    return 0;
}
