#TODO: jmp, braz, branz, scall
#TODO: negative value with imm

import re

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
        if self.get_opcode() == 0:
            return 0
        
        return self.register.get(self.instr_txt[1])
        
    def get_imm(self):
        if self.get_opcode() == 0:
            return 0
        
        if not self.instr_txt[2] in self.register:
            return 1
        else:
            return 0
    
    def get_o(self):
        if self.get_opcode() == 0:
            return 0
        if self.get_imm():
            return int(self.instr_txt[2])
        return self.register.get(self.instr_txt[2])
    
    def get_r_beta(self):
        if self.get_opcode() == 0:
            return 0
        return self.register.get(self.instr_txt[3])


def open_asm(asm_file):
    fd = open(asm_file, "r")
    return fd

def parse_asm(fd):
    comment = re.compile(r"^\s*#")
    label = re.compile(r"^\w")
    linebreak = re.compile(r"\n|\r")
    
    data = []
    for line in fd:
        if comment.match(line) or label.match(line) or linebreak.match(line):
                continue
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
    
    instr = encode.get_opcode() << 27
    instr += encode.get_r_alpha() << 22
    instr += encode.get_imm() << 21
    instr += encode.get_o() << 5
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