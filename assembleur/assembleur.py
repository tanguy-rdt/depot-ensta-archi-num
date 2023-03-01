import os
import re
import logging
from optparse import OptionParser


exit_code = 0
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
    
    
def options_parser():
    parser = OptionParser() 
    
    parser.add_option("-i", 
                      "--input", 
                      action="store", 
                      type="string",
                      dest="input_file", 
                      default="",
                      help="Path of the asm file to translate", 
                      metavar="INPUT_PATH")
    
    parser.add_option("-o", 
                    "--output", 
                    action="store", 
                    type="string",
                    dest="output_file", 
                    default="",
                    help="Path of the binary file", 
                    metavar="OUTUT_PATH")
    
    parser.add_option("-l", 
                    "--log", 
                    action="store_true", 
                    dest="log", 
                    default=False,
                    help="Genrate a log file")
    
    parser.add_option("-d", 
                    "--debug", 
                    action="store_true", 
                    dest="debug", 
                    default=False,
                    help="Debug mode")
    
    (options, args) = parser.parse_args()
    
    if (not options.input_file or not options.output_file):
        parser.print_help()
        exit()  
    
    return options.input_file, options.output_file, options.log, options.debug
    
    
def conf_logging(log, debug):
    logging_level = None
    if (debug):
        logging_level = logging.DEBUG
    else:
        logging_level = logging.INFO
        
    if (log):
        logging.basicConfig(filename='assembleur.log',
                            format='%(asctime)s -- %(levelname)s -- %(message)s',
                            datefmt='%Y-%m-%d, %H:%M:%S', 
                            level=logging_level)
    else:
        logging.basicConfig(format='%(asctime)s -- %(levelname)s -- %(message)s',
                            datefmt='%Y-%m-%d, %H:%M:%S', 
                            level=logging_level)


def open_asm(asm_path_file):
    logging.info("Opening the asm file: %s" %(asm_path_file))
    try:
        fd = open(asm_path_file, "r")
        return fd
    except Exception as err:
        logging.error("Unable to find the asm file: %s" %(err))
        global exit_code
        exit_code = 1
        return None
        
        
def parse_asm(fd):
    comment = re.compile(r"^\s*[#|;]")
    label = re.compile(r"^\w")
    linebreak = re.compile(r"\n|\r")
    
    logging.info("Parsing of the asm file")
    
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
    logging.info("Creation of the binary file: %s" %(bin_path_file))
    try:
        fd = open(bin_path_file, "w")
        return fd
    except Exception as err:
        logging.error("Unable to create the bin file: %s" %(err))
        global exit_code
        exit_code = 2
        return None
      
        
def get_instr_hex(instr_txt, instr_num):
    encode = EncodeInstr(instr_txt)
    opcode_txt = instr_txt[0]    
    
    instr = 0
    
    logging.debug("Conversion from str to hex of the instruction number %d: %s" %(instr_num, " ".join(instr_txt)))
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
        logging.debug("Result of the conversion: %s" %(instr))
        
        return instr
    except Exception as err:
        logging.error("Instruction number %d is not valid: %s" %(instr_num, err))
        global exit_code
        exit_code = 3
        return None
        
        
def append_to_bin_file(fd, instr_num, instr_hex):
    instr_num_hex = '0x{0:08X}'.format(instr_num)
    line = instr_num_hex + ' ' + instr_hex
    logging.debug("Append the instruction number %d in the binary file: %s" %(instr_num, instr_hex))
    try:
        fd.write(line + "\n")   
        return True
    except Exception as err:
        logging.error("Unable to write in the bin file: %s" %(err))
        global exit_code
        exit_code = 4
        return None
    
    
def main():
    input_file, output_file, log, debug = options_parser()
    conf_logging(log, debug)
            
    asm_path_file = os.path.join(os.getcwd(), input_file)
    bin_path_file = os.path.join(os.getcwd(), output_file)
    
    asm_fd = open_asm(asm_path_file)
    if asm_fd:
        instrs_txt = parse_asm(asm_fd)
        fd_binary = open_output_file(bin_path_file)
        if fd_binary:
            logging.info("Start of assembly to binary translation")
            for instr_txt in instrs_txt:
                instr_num = instrs_txt.index(instr_txt)
                instr_hex = get_instr_hex(instr_txt, instr_num)
                if instr_hex:
                    ret_append = append_to_bin_file(fd_binary, instr_num, instr_hex)
                    if not ret_append:
                        break
                else:
                    break
            logging.info("End of assembly to binary translation")
    
    logging.info("Exit code: %d" %(exit_code))
    exit(exit_code)        


if __name__ == '__main__':
    main()        