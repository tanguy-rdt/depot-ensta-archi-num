    add r0, 3, r1
    shl r1, 12, r1
    add r1, 255, r1
    shl r1, 12, r1
    add r1, 255, r1
    store r0, 0, r1

    add r0, 10, r2
    add r0, 200, r3
    add r0, 0, r4
    add r0, 1 r5
trace:
    add r0, 10, r1
    shl r1, 12, r1
    add r1, r2, r1
    add r2, 1, r2
    store r5, 0, r1
    add r5, 1 r5
    seq r2, r3, r4
    braz r4, trace



    scall 2


    stop