        add  r0, 2,  r1    # presenté dans le slideware 0x09c00043
        add  r0, 8,  r2    # additionner c'est bien
        add  r1, r2, r3
        sub  r2, r1, r4
        mul  r1, r2, r5
        div  r2, r1, r6
        and  r1, r4, r7
        or   r1, r4, r8
        xor  r1, r4, r9
        shl  r1, r2, r10
        shr  r2, r1, r11
        slt  r2, r1, r3
        sle  r1, r2, r4
        seq  r6, r9, r5
        stop