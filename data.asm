        add  r0, 2,  r1    # presenté dans le slideware 0x09c00043
        add  r0, -8,  r2    # additionner c'est bien
        add  r1, r2, r3
        add  r0, -4,  r31    # additionner c'est bien

label1:
        sub  r31, 2,  r31    # additionner c'est bien
        sub  r2, r1, r4
        mul  r1, r2, r5
        div  r2, r1, r6
        div  r1, r2, r12
        div  r1, 2, r13
        div  r2, -2, r14
        and  r1, r4, r7
        or   r1, r4, r8
        xor  r1, r4, r9
        shl  r1, r2, r10
        shr  r2, r1, r11
        branz r31, label1
        slt  r2, r1, r3
        sle  r1, r2, r4
        seq  r6, r9, r5
        load r0, 10, r20
        store r0, 21, r11
        stop