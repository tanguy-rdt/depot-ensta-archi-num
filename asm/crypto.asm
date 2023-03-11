    jmp give_nbr, r20
    jmp give_secret_nbr, r20
    xor r10, r11, r12
    jmp encrypted_nbr, r20

decrypt_nbr:
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
    add r0, 115, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 99, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 116, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r0, 110, r1
    scall 4
    add r0, 98, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r0, 116, r1
    scall 4
    add r0, 111, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r0, 100, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 99, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 121, r1
    scall 4
    add r0, 112, r1
    scall 4
    add r0, 116, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r12, 0, r1
    scall 1
    add r0, 58, r1
    scall 4
    add r0, 32, r1
    scall 4
    scall 0
    xor r1, r12, r13
    add r13, r0, r1
    scall 1
    add r0, 32, r1
    scall 4
    add r0, 45, r1
    scall 4
    add r0, 45, r1
    scall 4
    add r0, 62, r1
    scall 4
    add r0, 32, r1
    scall 4
    seq r13, r10, r2
    branz r2, success
    braz r2, failure

is_encrypt:
    braz r2, decrypt_nbr
    stop

is_decrypt:
    stop

give_nbr:
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
    add r0, 110, r1
    scall 4
    add r0, 98, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r0, 116, r1
    scall 4
    add r0, 111, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 110, r1
    scall 4
    add r0, 99, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 121, r1
    scall 4
    add r0, 112, r1
    scall 4
    add r0, 116, r1
    scall 4
    add r0, 58, r1
    scall 4
    add r0, 32, r1
    scall 4
    scall 0
    add r1, 0, r10
    jmp r20, r0

give_secret_nbr:
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
    add r0, 115, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 99, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 116, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r0, 110, r1
    scall 4
    add r0, 98, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 58, r1
    scall 4
    add r0, 32, r1
    scall 4
    scall 0 
    add r1, r0, r11
    jmp r20, r0

encrypted_nbr:
    add r0, 89, r1
    scall 4
    add r0, 111, r1
    scall 4
    add r0, 117, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 110, r1
    scall 4
    add r0, 99, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 121, r1
    scall 4
    add r0, 112, r1
    scall 4
    add r0, 116, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 100, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r0, 110, r1
    scall 4
    add r0, 98, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 58, r1
    scall 4
    add r0, 32, r1
    scall 4
    add r12, r0, r1
    scall 1
    add r0, 10, r1
    scall 4
    jmp r20, r0

failure:
    add r0, 70, r1
    scall 4
    add r0, 97, r1
    scall 4
    add r0, 105, r1
    scall 4
    add r0, 108, r1
    scall 4
    add r0, 117, r1
    scall 4
    add r0, 114, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 10, r1
    scall 4
    jmp is_encrypt, r0

success:
    add r0, 83, r1
    scall 4
    add r0, 117, r1
    scall 4
    add r0, 99, r1
    scall 4
    add r0, 99, r1
    scall 4
    add r0, 101, r1
    scall 4
    add r0, 115, r1
    scall 4
    add r0, 115, r1
    scall 4
    add r0, 10, r1
    scall 4
    jmp is_encrypt, r0


