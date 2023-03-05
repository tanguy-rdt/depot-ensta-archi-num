    add r0, 0, r1 ; cnt memory
    
    ; initialisation de la mem
    add r0, 3, r2 ; multiplicateur de pixels (pour voir plus gros)
    shl r2, 12, r2 ; décalage de la position du facteur pour respecter 0x0fxxxyyy
    add r2, 255, r2 ; résolution en pixels sur les x (largeur)
    shl r2, 12, r2
    add r2, 255, r2 ; résolution en pixels sur les y (hauteur)
    store r1, 0, r2 ; registre init dans la mem à addr 0 (registre utilisé par l'écran)
    add r1, 1, r1 ; on incrémente le compteur de mem

    add r0, 10, r2 ; position x de départ
    add r0, 50, r3 ; position x de fin 
    add r0, 10, r4 ; position y de départ
    add r0, 0, r5 ; indicateur de limite x atteinte (1 oui, 0 non)
    jmp trace_horizontal, r20

    add r0, 10, r2 ; position x de départ
    add r0, 50, r3 ; position x de fin 
    add r0, 50, r4 ; position y de départ
    add r0, 0, r5 ; indicateur de limite x atteinte (1 oui, 0 non)
    jmp trace_horizontal, r20

    add r0, 10, r2 ; position x de départ
    add r0, 10, r3 ; position y de départ 
    add r0, 50, r4 ; position y de fin
    add r0, 0, r5 ; indicateur de limite x atteinte (1 oui, 0 non)
    jmp trace_vertical, r20

    add r0, 50, r2 ; position x de départ
    add r0, 10, r3 ; position y de départ 
    add r0, 51, r4 ; position y de fin
    add r0, 0, r5 ; indicateur de limite x atteinte (1 oui, 0 non)
    jmp trace_vertical, r20

    add r0, 10, r2 ; position x de départ
    add r0, 10, r3 ; position y de départ 
    add r0, 51, r4 ; position x de fin
    add r0, 51, r5 ; position y de fin
    add r0, 0, r6 ; indicateur de limite x atteinte (1 oui, 0 non)
    jmp trace_diagonal_right, r20

    add r0, 50, r2 ; position x de départ
    add r0, 10, r3 ; position y de départ 
    add r0, 10, r4 ; position x de fin
    add r0, 51, r5 ; position y de fin
    add r0, 0, r6 ; indicateur de limite x atteinte (1 oui, 0 non)
    jmp trace_diagonal_left, r20

    scall 2
    stop


trace_horizontal:
    add r0, r2, r6 ; position x (évolue)
    shl r6, 12, r6 ; décalage de la position x pour respecter 0x00xxxyyy
    add r6, r4, r6 ; position de y (constant)
    store r1, 0, r6 ; on charge le pixel dans la mem
    add r1, 1, r1 ; on incrémente le compteur d'addr de mem
    add r2, 1, r2 ; on incrémente de 1 la position x
    seq r2, r3, r5 ; on regarde si x à atteint la position final
    braz r5, trace_horizontal ; si c'est pas le cas on va a trace_horizontal
    jmp r20, r0

trace_vertical:
    add r0, r2, r6 ; position x (constant)
    shl r6, 12, r6 ; décalage de la position x pour respecter 0x00xxxyyy
    add r6, r3, r6 ; position de y (évolue)
    store r1, 0, r6 ; on charge le pixel dans la mem
    add r1, 1, r1 ; on incrémente le compteur d'addr de mem
    add r3, 1, r3 ; on incrémente de 1 la position x
    seq r3, r4, r5 ; on regarde si y à atteint la position final
    braz r5, trace_vertical ; si c'est pas le cas on va a trace_horizontal
    jmp r20, r0

trace_diagonal_right:
    add r0, r2, r6 ; position x (évolue)
    shl r6, 12, r6 ; décalage de la position x pour respecter 0x00xxxyyy
    add r6, r3, r6 ; position de y (évolue)
    store r1, 0, r6 ; on charge le pixel dans la mem
    add r1, 1, r1 ; on incrémente le compteur d'addr de mem
    add r2, 1, r2 ; on incrémente de 1 la position x
    add r3, 1, r3 ; on incrémente de 1 la position y
    seq r2, r4, r6 ; on regarde si x à atteint la position final
    braz r6, trace_diagonal_right ; si c'est pas le cas on va a trace_diagonal_right
    jmp r20, r0

trace_diagonal_left:
    add r0, r2, r6 ; position x (évolue)
    shl r6, 12, r6 ; décalage de la position x pour respecter 0x00xxxyyy
    add r6, r3, r6 ; position de y (évolue)
    store r1, 0, r6 ; on charge le pixel dans la mem
    add r1, 1, r1 ; on incrémente le compteur d'addr de mem
    sub r2, 1, r2 ; on décrémente de 1 la position x
    add r3, 1, r3 ; on incrémente de 1 la position y
    seq r2, r4, r6 ; on regarde si x à atteint la position final
    braz r6, trace_diagonal_left ; si c'est pas le cas on va a trace_diagonal_left
    jmp r20, r0