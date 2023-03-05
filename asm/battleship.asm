    
    add r0, 1, r1
    shl r1, 8, r1
    add r0, 30, r1
    shl r1, 8, r1
    add r0, 255, r1
    store r0, 0, r1

    add r0, 1, r1
    add r0, 15, r2
    add r0, 15, r3
init:
    sub r2, 1, r2
    add r0, r2, r4
    shl r4, 4, r4
    add r4, r3, r4
    shl r4, 8, r4
    add r4, 255, r4
    shl r4, 8, r4
    add r4, 255, r4
    shl r4, 8, r4
    add r4, 255, r4
    store r1, 0, r4
    add r1, 1, r1
    branz r2, init
    sub r3, 1, r3
    add r0, 15, r2
    seq r3, -1, r5
    braz r5, init

    scall 2

    stop 