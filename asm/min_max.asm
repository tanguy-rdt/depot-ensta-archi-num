       ; il faut compiler avec le mode verbose make VERBOSE=1
       
        add r0, 32767, r1
        mul r1, 32767, r2
        mul r2, 2, r3
        mul r1, 4, r4
        add r4, 1, r4
        add r4, r3, r5


        add r0, -1, r10
        mul r10, r5, r10
        add r10, -1, r10
        stop