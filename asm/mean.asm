    jmp give_value, r20
    add r0, r1, r2
    jmp give_value, r20
    add r0, r1, r3
    jmp mean, r20
    add r2, r3, r4
    div r4, 2, r1
    scall 1
    stop

give_value:
    add r0, 71, r1
    scall 4
    add r0, 105, r1
    scall 4
    add r0, 118, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r0, 118, r1
    scall 4
    add r0, 97, r1
    scall 4
    add r0, 108, r1
    scall 4
    add r0, 117, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r0, 58, r1
    scall 4
    add r0, 32, r1
    scall 4
    scall 0
    jmp r20, r0

mean:
    add r0, 77, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 97, r1
    scall 4
    add r0, 110, r1
    scall 4
    add r0, 58, r1
    scall 4
    add r0, 32, r1
    scall 4
    jmp r20, r0