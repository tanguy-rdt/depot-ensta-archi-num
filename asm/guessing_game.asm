    jmp guessing_game, r20
    add r0, 50, r2
    scall 4
    add r1, 0, r2

in_game:
    jmp give_value, r20
    jmp test_value, r20
    jmp clue, r21
    braz r3, in_game
    jmp win, r20
    stop

guessing_game: 
    add r0, 71, r1
    scall 3
    add r0, 117, r1
    scall 3
    add r0, 101, r1
    scall 3
    add r0, 115, r1
    scall 3
    add r0, 115, r1
    scall 3
    add r0, 105, r1
    scall 3
    add r0, 110, r1
    scall 3
    add r0, 103, r1
    scall 3
    add r0, 32, r1
    scall 3
    add r0, 103, r1
    scall 3
    add r0, 97, r1
    scall 3
    add r0, 109, r1
    scall 3
    add r0, 101, r1
    scall 3
    add r0, 32, r1
    scall 3
    add r0, 33, r1
    scall 3
    add r0, 10, r1
    scall 3
    add r0, 10, r1
    scall 3
    add r0, 10, r1
    scall 3
    jmp r20, r0

give_value:
    add r0, 71, r1
    scall 3
    add r0, 105, r1
    scall 3
    add r0, 118, r1
    scall 3
    add r0, 101, r1
    scall 3
    add r0, 32, r1
    scall 3
    add r0, 118, r1
    scall 3
    add r0, 97, r1
    scall 3
    add r0, 108, r1
    scall 3
    add r0, 117, r1
    scall 3
    add r0, 101, r1
    scall 3
    add r0, 58, r1
    scall 3
    add r0, 32, r1
    scall 3
    scall 0
    jmp r20, r0

test_value:
    seq r1, r2, r3
    slt r1, r2, r4
    or r3, r4, r5
    jmp r20, r0

is_equal:
    add r0, 61, r1
    jmp print_clue, r0

is_inf:
    add r0, 45, r1
    jmp print_clue, r0

is_sup:
    add r0, 43, r1
    jmp print_clue, r0

clue:
    add r0, 73, r1
    scall 3
    add r0, 116, r1
    scall 3
    add r0, 39, r1
    scall 3
    add r0, 115, r1
    scall 3
    add r0, 58, r1
    scall 3
    add r0, 32, r1
    scall 3
    branz r3, is_equal
    branz r4, is_sup
    braz r5, is_inf

print_clue:
    scall 3
    add r0, 10, r1
    scall 3
    add r0, 10, r1
    scall 3
    jmp r21, r0

win:
    add r0, 89, r1
    scall 3
    add r0, 111, r1
    scall 3
    add r0, 117, r1
    scall 3
    add r0, 32, r1
    scall 3
    add r0, 119, r1
    scall 3
    add r0, 105, r1
    scall 3
    add r0, 110, r1
    scall 3
    add r0, 32, r1
    scall 3
    add r0, 33, r1
    scall 3
    add r0, 10, r1
    scall 3
    jmp r20, r0
