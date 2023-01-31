import re

comment = re.compile(r"^#")
label = re.compile(r"^\w")



with open("data.asm", "r") as fp:
    data = []
    for line in fp:
        if comment.match(line) or label.match(line):
                continue
        else:
            instr=[]
            line = line.replace("\n", "")
            line = line.replace(" ", ",")
            line = line.split(",")   
            print(line)      
            instr.append(line)
            
        data.append(instr)
fp.close()
        
#print(data)

        
        