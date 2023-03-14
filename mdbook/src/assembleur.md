# Assembleur

- [Assembleur](#assembleur)
  - [Utilisation](#utilisation)
  - [Options](#options)
  - [Syntaxe asm](#syntaxe-asm)
  - [Gestion des Erreurs](#gestion-des-erreurs)

## Utilisation 

- Depuis la racine du projet __sans__ `set-env`
   ```bash
   $ python3 assembleur/assembleur -i asm/guessing_game.asm -o bin/guessing_game.bin
   ```

- Depuis la racine du projet __avec__ `set-env`
   ```bash
   $ assembleur -i asm/guessing_game.asm -o bin/guessing_game.bin
   ```


Il est nécessaire de spécifier le chemin du fichier asm avec l'option `-i` et le chemin du fichier binaire avec `-o`. Sans ces informations le programme ne pourra pas s'exécuter.


## Options 

```
$ python3 assembleur.py -h
Usage: assembleur.py [options]

Options:
  -h, --help            show this help message and exit
  -i INPUT_PATH, --input=INPUT_PATH
                        Path of the asm file to translate
  -o OUTUT_PATH, --output=OUTUT_PATH
                        Path of the binary file
  -l, --log             Generate a log file
  -d, --debug           Debug mode
```

1. __`-l --log`__:\
   Permet de diriger la sortie standard du programme vers le fichier _assembleur.log_ \
   __Exemple:__
   ```
   $ assembleur -i asm/guessing_game.asm -o bin/guessing_game.bin -l  
   $ tail -n 2 assembleur/assembleur.log
   2023-02-26, 22:27:23 -- INFO -- End of assembly to binary translation
   2023-02-26, 22:27:23 -- INFO -- Exit code: 0
   ```

2. __`-d --debug`__:\
   Permet d'obtenir plus d'information pendant l'exécution du programme si des problèmes surviennent. \
   __Exemple:__
   ```
   $ assembleur -i asm/guessing_game.asm -o bin/guessing_game.bin -d
   2023-02-26, 22:25:26 -- INFO -- Opening the asm file: /home/me/depot-ensta-archi-num/asm/guessing_game.asm
   2023-02-26, 22:25:26 -- INFO -- Parsing of the asm file
   2023-02-26, 22:25:26 -- INFO -- Creation of the binary file: /home/me/depot-ensta-archi-num/bin/guessing_game.bin
   2023-02-26, 22:25:26 -- INFO -- Start of assembly to binary translation
   2023-02-26, 22:25:26 -- DEBUG -- Conversion from str to hex of the instruction number 0: add r0 2 r1
   2023-02-26, 22:25:26 -- DEBUG -- Result of the conversion: 0x08200041
   2023-02-26, 22:25:26 -- DEBUG -- Append the instruction number 0 in the binary file: 0x08200041
   2023-02-26, 22:25:26 -- DEBUG -- Conversion from str to hex of the instruction number 1: adQ r0 8 r2
   2023-02-26, 22:25:26 -- ERROR -- Instruction number 1 is not valid: unsupported operand type(s) for <<: 'NoneType' and 'int'
   2023-02-26, 22:25:26 -- INFO -- End of assembly to binary translation
   2023-02-26, 22:25:26 -- INFO -- Exit code: 3
   ```

## Syntaxe asm
1. __Commentaire__:
   - Les commentaires sont identifiés avec le caractère `;`, tout texte qui suit ce caractère sera ignoré 
   - Le commentaire peut être collé ou espacé du caractère `;`
   - Ils peuvent être placé à n'importe quel endroit
  
2. __Label__:
   - Les labels doivent contenir au moins un caractère suivit de `:`, sans espace
   - Les labels doivent obligatoirement être précedé d'un retour à la ligne, il est important que ce soit le premier élément de la ligne
   - Les labels peuvent être suivit d'un commentaire, d'une instruction, d'espaces ou d'un retour à la ligne
  
3. __Instruction__:
   - Les instructions peuvent être précédé par un label
   - Les instructions doivent obligatoirement être précédé d'une tabulation si la ligne contient uniquement l'instruction et optionellement un commentaire

__Exemple:__
   ```asm
         ; add  r0, 4,  r31 
   label2: sub  r31, 2,  r31 ; comment
         sub  r2, r1, r4

   label4: ; comment
         sub  r2, r1, r4

         stop
   ```


## Gestion des Erreurs

Code de sortie | Information 
---------------|------------
0              | Programme exécuter avec succès
1              | Option -i et/ou -o non précisé
2              | Erreur lors de l'ouverture du fichier asm
3              | Erreur lors de la création du fichier binaire
4              | Erreur lors de la convertion de l'instruction en hexadécimal
5              | Erreur lors de l'écriture dans le fichier binaire
