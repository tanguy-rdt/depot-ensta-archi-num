# ISS

- [ISS](#iss)
  - [Utilisation](#utilisation)
  - [Fonctionnalités](#fonctionnalités)
  - [Option](#option)
  - [Mémoire](#mémoire)
  - [Gestion des Erreurs](#gestion-des-erreurs)
  - [MIPS](#mips)

## Utilisation 

```
$ iss
Usage:
        ./iss <bin_file_path>
```

- Depuis la racine du projet __sans__ `set-env`
   ```bash
   $ make -C simulateur
   $ ./simulateur/iss bin/guessing_game.bin
   ```

- Depuis la racine du projet __avec__ `set-env`
   ```bash
   $ make -C simulateur
   $ iss bin/guessing_game.bin
   ```


Il est nécessaire de spécifier le chemin du fichier binaire. Sans cette information le programme ne pourra pas s'exécuter. \
Le programme ne prends pas d'autres arguments, si plus d'un argument est donné alors le programme générera une erreur.

## Fonctionnalités

1. __Opération décimal:__
   add, sub, mul, div

2. __Opération binaire:__
   or, and, xor, shl, shr

3. __Tests sur des valeurs:__
   slt, sle, seq

4. __Branchement:__
   jmp, braz, branz

5. __Mémoire:__
   load, store

6. __Appel system:__
   
   scall n | Informations
   --------|-------------
   scall 0 | Demande d'un nombre décimal, enregistré dans _r1_
   scall 1 | Affichage d'un nombre décimal dans _r1_
   scall 2 | Affichage d'un écran 
   scall 3 | Demande d'un caractère, enregistré dans _r1_
   scall 4 | Affichage d'un caractère dans _r1_
   scall 5 | Génération d'un nombre alléatoire dans _r1_

## Option 

1. __`VERBOSE=1`__:\
   Permet d'utiliser l'iss en mode verbose. Si cette option est utilisé alors l'instruction réalisée et l'état des registres seront affichés à chaque nouvelle instruction. \
   Cette option ne s'utilise pas avec l'éxecutable, elle doit être donné au moment de la compilation: `make VERBOSE=1`

   __Exemple:__
   ```
   $ make -C simulateur VERBOSE=1
   $ iss bin/square.bin
   jmp r20 0
   regs = 00000000 000000f3 0000000a 00000032 0000000a 00000033 00000001 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000028 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
   Cycle number = 26120

   scall 2
   regs = 00000000 000000f3 0000000a 00000032 0000000a 00000033 00000001 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000028 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
   Cycle number = 26120

   stop
   regs = 00000000 000000f3 0000000a 00000032 0000000a 00000033 00000001 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000028 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
   Cycle number = 26120


   Elapsed time 73269us for 26120 cycle
   Estimated MIPS: 356494 million par seconde
   ```

## Mémoire

Notre iss est fournis avec quatre type de mémoire:

1. __La mémoire d'instructions:__ 2048 octets, nos instructions sont sur 4 octets ce qui nous permet de stocker 512 instructions.
2. __La mémoire de données:__ 128 octets, soit 32 registres sur 32 bits.
3. __La mémoire disque:__ 1024 octets, soit 256 registres sur 32 bits.
4. __La mémoire cache:__ 512 octets, soit 128 registres sur 32 bits.


## Gestion des Erreurs

Code de sortie | Information 
---------------|------------
0              | Programme executer avec succès
1              | Fichier binaire non précisé
2              | Erreur lors de l'ouverture du fichier binaire
3              | Mémoire d'instruction dépassée
4              | Dépassement du registre _(overflow)_


## MIPS

Pour réaliser une mesure du _MIPS_ on vous conseil de compiler l'iss sans le mode verbose (`make -C simulateur`) pour améliorer la rapidité d'éxecution. \
De plus, vous avez à dipsosition le fichier _mips.asm_ qui permet de réaliser un grand nombre d'instructions _(décrémentation de `0x7fffffff` jusqu'à `0x0`)_. La duré d'éxecution de ce programme peut prendre un certain temps, plus de 4 milliards d'instructions sont réalisés. \

A titre de comparatif voici les différents résultat que nous avons pu obtenir :

CPU | MIPS
----|----
Apple Silicon M1 (3,2GHz) | 150 MIPS
i5-8257U (TurboBoost 3,9GHz) | 117 MIPS
i7-2600 (OC 3,8GHz) | 90 MIPS

