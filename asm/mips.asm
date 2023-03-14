        add r0, 32767, r1
        mul r1, 32767, r2
        mul r2, 2, r3
        mul r1, 4, r4
        add r4, 1, r4
        add r4, r3, r1

mips:
    sub r1, 1, r1
    branz r1, mips
    
    stop