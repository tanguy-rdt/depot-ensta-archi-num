        add r0, 30, r3
        add r0, 2, r1

max_positif:
        mul r1, 2, r1
        sub r3, 1, r3
        branz r3, max_positif
        sub r1, 1, r1



        add r0, 30, r3
        add r0, -2, r2

max_negatif:
        mul r2, 2, r2
        sub r3, 1, r3
        branz r3, max_negatif

        scall 1
        add r0, r2, r1
        scall 1
        stop


        ;add r0, 32767, r1
        ;mul r1, 32767, r2
        ;mul r2, 2, r3
        ;mul r1, 4, r4
        ;add r4, 1, r4
        ;add r4, r3, r5


        ;add r0, -32768, r10
        ;mul r10, 32768, r12
        ;mul r12, 2, r13
        ;add r14, 1, r14
        ;add r14, r13, r15
        ;stop