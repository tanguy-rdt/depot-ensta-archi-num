#TODO: braz, branz: label
#TODO: negative value with imm

import re

label_addr = {}


class EncodeInstr:
    def __init__ (self, instr_txt):
        self.instr_txt = instr_txt
        
        self.opcode = {}
        self.opcode["stop"]  = 0
        self.opcode["add"]   = 1
        self.opcode["sub"]   = 2
        self.opcode["mul"]   = 3
        self.opcode["div"]   = 4
        self.opcode["and"]   = 5
        self.opcode["or"]    = 6
        self.opcode["xor"]   = 7
        self.opcode["shl"]   = 8
        self.opcode["shr"]   = 9
        self.opcode["slt"]   = 10
        self.opcode["sle"]   = 11
        self.opcode["seq"]   = 12
        self.opcode["load"]  = 13
        self.opcode["store"] = 14
        self.opcode["jmp"]   = 15
        self.opcode["braz"]  = 16
        self.opcode["branz"] = 17
        self.opcode["scall"] = 18

        self.register = {}
        self.register["r0"] = 0
        self.register["r1"] = 1
        self.register["r2"] = 2
        self.register["r3"] = 3
        self.register["r4"] = 4
        self.register["r5"] = 5
        self.register["r6"] = 6
        self.register["r7"] = 7
        self.register["r8"] = 8
        self.register["r9"] = 9
    
    def get_opcode(self):
        return self.opcode.get(self.instr_txt[0])
    
    def get_r_alpha(self):
        return self.register.get(self.instr_txt[1])
        
    def get_imm(self, index_o):
        if not self.instr_txt[index_o] in self.register:
            return 1
        else:
            return 0
    
    def get_o(self, index_o):
        if self.instr_txt[index_o] in label_addr: # si c'est un label 
            return label_addr.get(self.instr_txt[index_o])
        elif self.get_imm(index_o):
            return int(hex(int(self.instr_txt[index_o]) + (1<<16)), 16)
        return self.register.get(self.instr_txt[index_o])
    
    def get_r_beta(self):
        return self.register.get(self.instr_txt[3])
    
    def get_r(self, index_r):
        return self.register.get(self.instr_txt[index_r])
            
    def get_a(self):
        if self.instr_txt[2] in label_addr:
            return label_addr.get(self.instr_txt[2])
        return int(self.instr_txt[2])

    def get_n(self):
        return int(self.instr_txt[1])  
    


def open_asm(asm_file):
    fd = open(asm_file, "r")
    return fd

def parse_asm(fd):
    comment = re.compile(r"^\s*#")
    label = re.compile(r"^\w")
    linebreak = re.compile(r"\n|\r")
    
    data = []
    for line in fd:        
        if comment.match(line) or linebreak.match(line):
            continue
        if label.match(line):
            label_addr[line[:line.index(':')]] = len(data)
        else:
            line = line.replace(',', ' ')
            line = line.split()  
            if '#' in line:
                line = line[:line.index('#')]
            data.append(line)
    fd.close()
            
    return data

def open_bin_file():
    fd = open("data.bin", "w")
    return fd

def get_instr_hex(instr_txt):
    encode = EncodeInstr(instr_txt)
    opcode_txt = instr_txt[0]    
    
    instr = 0
    if opcode_txt == "jmp":
        instr += encode.get_opcode() << 27
        instr += encode.get_imm(index_o=1) << 26
        instr += encode.get_o(index_o=1) << 5
        instr += encode.get_r(index_r=2)
    elif opcode_txt == "braz" or opcode_txt == "branz":
        instr += encode.get_opcode() << 27
        instr += encode.get_r(index_r=1) << 22
        instr += encode.get_a()
    elif opcode_txt == "scall":
        instr += encode.get_opcode() << 27
        instr += encode.get_n()
    elif opcode_txt == "stop":
        instr += encode.get_opcode() << 27
    else:
        instr += encode.get_opcode() << 27
        instr += encode.get_r_alpha() << 22
        instr += encode.get_imm(index_o=2) << 21
        instr += encode.get_o(index_o=2) << 5
        instr += encode.get_r_beta() 
    
    instr = '0x{0:08X}'.format(instr)
    
    return instr

def append_to_bin_file(fd, instr_num, instr_hex):
    instr_num_hex = '0x{0:08X}'.format(instr_num)
    line = instr_num_hex + ' ' + instr_hex
    fd.write(line + "\n")    


    
def main():
    asm_fd = open_asm("data.asm")
    instrs_txt = parse_asm(asm_fd)
    fd_binary = open_bin_file()
    for instr_txt in instrs_txt:
        instr_hex = get_instr_hex(instr_txt)
        append_to_bin_file(fd_binary, instrs_txt.index(instr_txt), instr_hex)
        

if __name__ == '__main__':
    main()        