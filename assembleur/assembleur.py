#TODO: jmp, braz, branz, scall
#TODO: negative value with imm

import re

opcode = {}
opcode["stop"]  = 0
opcode["add"]   = 1
opcode["sub"]   = 2
opcode["mul"]   = 3
opcode["div"]   = 4
opcode["and"]   = 5
opcode["or"]    = 6
opcode["xor"]   = 7
opcode["shl"]   = 8
opcode["shr"]   = 9
opcode["slt"]   = 10
opcode["sle"]   = 11
opcode["seq"]   = 12
opcode["load"]  = 13
opcode["store"] = 14

register = {}
register["r0"] = 0
register["r1"] = 1
register["r2"] = 2
register["r3"] = 3
register["r4"] = 4
register["r5"] = 5
register["r6"] = 6
register["r7"] = 7
register["r8"] = 8
register["r9"] = 9


def load_asm(asm_file):
    comment = re.compile(r"^\s*#")
    label = re.compile(r"^\w")
    linebreak = re.compile(r"\n|\r")

    with open(asm_file, "r") as fp:
        data = []
        for line in fp:
            if comment.match(line) or label.match(line) or linebreak.match(line):
                    continue
            else:
                line = line.replace(',', ' ')
                line = line.split()  
                if '#' in line:
                    line = line[:line.index('#')]
                data.append(line)
    return data


def open_bin_file():
    fd = open("data.bin", "w")
    return fd

def get_instr_hex(instr_text):
    imm = 0
    if instr_text[0] == "stop":
        instr = 0
        instr = '0x{0:08X}'.format(instr)
        return instr
    
    if not instr_text[2] in register:
        imm = 1
        
    instr = opcode.get(instr_text[0]) << 27
    instr += register.get(instr_text[1]) << 22
    instr += imm << 21
    if imm:
        instr += int(instr_text[2]) << 5
    else :
        instr += register.get(instr_text[2]) << 5
    instr += register.get(instr_text[3])
    instr = '0x{0:08X}'.format(instr)
    
    return instr

def append_to_bin_file(fd, instr_num, instr_hex):
    instr_num_hex = '0x{0:08X}'.format(instr_num)
    line = instr_num_hex + ' ' + instr_hex
    fd.write(line + "\n")    


    
def main():
    instrs = load_asm("data.asm")
    fd_binary = open_bin_file()
    for instr in instrs:
        instr_hex = get_instr_hex(instr)
        append_to_bin_file(fd_binary, instrs.index(instr), instr_hex)
        

if __name__ == '__main__':
    main()        