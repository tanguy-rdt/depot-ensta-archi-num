import os
import re

from optparse import OptionParser
from pathlib import Path

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
        for i in range(32):
            register_num = 'r' + str(i)
            self.register[register_num] = i
    
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
            return int(hex((int(self.instr_txt[index_o]) + (1<<16)) & 0x0000ffff), 16)
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
    


def open_asm(asm_path_file):
    try:
        fd = open(asm_path_file, "r")
        return fd
    except Exception as err:
        print("ERROR: Unable to find the asm file\n\t", err)
        exit(2)

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

def open_output_file(bin_path_file):
    try:
        fd = open(bin_path_file, "w")
        return fd
    except Exception as err:
        print("ERROR: Unable to create the bin file\n\t", err)
        exit(2)

def get_instr_hex(instr_txt, instr_num):
    encode = EncodeInstr(instr_txt)
    opcode_txt = instr_txt[0]    
    
    instr = 0
    
    try:
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
    except Exception as err:
        print("ERROR: Instruction number %d is not readable\n\t%s" %(instr_num, err))
        exit(3)

def append_to_bin_file(fd, instr_num, instr_hex):
    instr_num_hex = '0x{0:08X}'.format(instr_num)
    line = instr_num_hex + ' ' + instr_hex
    try:
        fd.write(line + "\n")   
    except Exception as err:
        print("ERROR: Unable to write in the bin file\n\t", err)
        exit(3)            
    
def main():
    parser = OptionParser() 
    
    parser.add_option("-i", 
                      "--input", 
                      action="store", 
                      type="string",
                      dest="input_file", 
                      help="Path of the asm file to translate", 
                      metavar="INPUT_PATH")
    
    parser.add_option("-o", 
                    "--output", 
                    action="store", 
                    type="string",
                    dest="output_file", 
                    help="Path of the binary file", 
                    metavar="OUTUT_PATH")
    
    (options, args) = parser.parse_args()
    
    if (not options.input_file or not options.output_file):
        parser.print_help()
        exit(1)     
        
    asm_path_file = os.path.abspath(os.path.join(os.path.dirname(__file__), options.input_file))
    asm_fd = open_asm(asm_path_file)
    instrs_txt = parse_asm(asm_fd)
    
    bin_path_file = os.path.abspath(os.path.join(os.path.dirname(__file__), options.output_file))
    fd_binary = open_output_file(bin_path_file)
        
    for instr_txt in instrs_txt:
        instr_num = instrs_txt.index(instr_txt)
        instr_hex = get_instr_hex(instr_txt, instr_num)
        append_to_bin_file(fd_binary, instr_num, instr_hex)
    
    exit(0)
        

if __name__ == '__main__':
    main()        