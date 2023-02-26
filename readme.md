# Structure de fichier

```
./
├── asm
│   └── some asm example
├── assembleur
│   └── assembleur.py
├── bin
│   └── some bin example
├── readme.md
└── simulateur
    ├── cache
    │   ├── cache.cpp
    │   └── cache.h
    ├── iss.c
    ├── Makefile
    └── memory
        ├── memory.cpp
        └── memory.h
```
    
# Assembleur

## Utilisation 

```bash
$ cd assembleur
$ python3 assembleur.py -i ../asm/data.asm -o ../bin/data.bin
```

Il est nécessaire de spécifier le chemin du fichier asm avec l'option `-i` et le chemin du fichier binaire avec `-o`. Sans ces informations le programme ne pourra pas s'executer.


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
   Permet de diriger la sortie standart du programme vers le fichier _assembleur.log_ \
   __Exemple:__
   ```
   $ python assembleur.py -i ../asm/data.asm -o ../bin/data.bin -l  
   $ tail -n 2 assembleur.log
   2023-02-26, 22:27:23 -- INFO -- End of assembly to binary translation
   2023-02-26, 22:27:23 -- INFO -- Exit code: 0
   ```

2. __`-d --debug`__:\
   Permet d'obtenir plus d'information pendant l'execution du programme si des problèmes surviennent. \
   __Exemple:__
   ```
   $ python assembleur.py -i ../asm/data.asm -o ../bin/data.bin -d
   2023-02-26, 22:25:26 -- INFO -- Opening the asm file: /Users/tanguyrdt/Documents/ENSTA_depot/depot-ensta-archi-num/asm/data.asm
   2023-02-26, 22:25:26 -- INFO -- Parsing of the asm file
   2023-02-26, 22:25:26 -- INFO -- Creation of the binary file: /Users/tanguyrdt/Documents/ENSTA_depot/depot-ensta-archi-num/bin/data.bin
   2023-02-26, 22:25:26 -- INFO -- Start of assembly to binary translation
   2023-02-26, 22:25:26 -- DEBUG -- Conversion from str to hex of the instruction number 0: add r0 2 r1
   2023-02-26, 22:25:26 -- DEBUG -- Result of the conversion: 0x08200041
   2023-02-26, 22:25:26 -- DEBUG -- Append the instruction number 0 in the binary file: 0x08200041
   2023-02-26, 22:25:26 -- DEBUG -- Conversion from str to hex of the instruction number 1: adQ r0 8 r2
   2023-02-26, 22:25:26 -- ERROR -- Instruction number 1 is not valid: unsupported operand type(s) for <<: 'NoneType' and 'int'
   2023-02-26, 22:25:26 -- INFO -- End of assembly to binary translation
   2023-02-26, 22:25:26 -- INFO -- Exit code: 3
   ```
## Gestion des Erreurs

Code de sortie | Information 
---------------|------------
0              | Programme executer avec succés
1              | Erreur lors de l'ouverture du fichier asm
2              | Erreur lors de la création du fichier binaire
3              | Erreur lors de la convertion de l'instruction en hexadécimal
4              | Erreur lors de l'écriture dans le fichier binaire

# ISS