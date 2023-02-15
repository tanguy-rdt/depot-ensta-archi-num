#TODO:

1. ne pas oublier de bloquer r0 a 0
2. mettre bien les types de variables
3. allocation dynamique
4. and, or, xor j'ai pas l'impression que ça marche. Je pense il faudrait les convertires en binaires
parce que 0010 (2) | 0110 (6) = 8 --> 1000 (8) et pas 0110 (6), il ne retient pas la retenu en faite
5. vérifier que load est store sont correcte, si on simule ou ne passe pas par une mémoire alors c'est bon
6. add = un cycle -> compteur de cycle +1
7. load ou store = 100 cycles -> compteur de cycle +100
8. modéliser les caches 