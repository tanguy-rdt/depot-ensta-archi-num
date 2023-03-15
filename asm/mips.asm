        add r0, 32767, r1
        add r1, 32767, r1
        mul r1, 32767, r1

mips:
    sub r1, 1, r1
    branz r1, mips
    
    stop