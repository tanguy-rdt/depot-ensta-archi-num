import re

def load_asm(asm_file):
    comment = re.compile(r"^#")
    label = re.compile(r"^\w")
    linebreak = re.compile(r"\n")

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
    
def main():
    data = load_asm("data.asm")
    print(data)
    


if __name__ == '__main__':
    main()        