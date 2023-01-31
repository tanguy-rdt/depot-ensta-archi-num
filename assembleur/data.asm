#test 1 asm : dummy code to test all instruction conversion
        add  r7, r2,r3    # presenté dans le slideware 0x09c00043
        add  r1, r2,r3    # additionner c'est bien
        sub  r4,  5,r6

label1:
        mul  r7,  8,r8    # multiplier c'est mieux
        div  r1, r2,r3
        and  r4,  5,r6
        or   r7, 18,r9
        xor  r1, r2,r3
        shl  r4,  5,r6
        seq  r7, 18,r9
        load r4,  5,r6

label2: 
        store r7, 18,r9
#        braz  r0, label1
#        branz r2, label2
        sub r3,-42,r3

label3:
 #       jmp r2, r0
        stop              # hey
        add r1,-1,r2
