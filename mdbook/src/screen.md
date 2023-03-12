# Screen 

Notre écran s'utilise à l'aide de l'appel system "_scall 5_". \
Pour pouvoir l'utiliser il est nécessaire d'avoir _PySide6_ :

```bash
$ pip install pyside6
```

## screen.py
Cette écran à était conçus principalement pour réaliser des dessins ou des graphiques, il n'est pas réellement adapter pour afficher une image monochrome.

_Exemple avec `square.asm`:_

<p align="center">
  <img src="./.img/screen.png" alt="square.asm">
</p>



#### Initialisation 
La première ligne de la mémoire, adresse `0x00000000`, permet d'initialiser l'écran.

Registre d'initialisation :

<p align="center">
  <img src="./.img/screen_reg_init.png" alt="screen_reg_init">
</p>

- `f` : Facteur de taille, permet d'agrandir la taille d'un pixel
- `x` : Nombre de pixels en largeur
- `y` : Nombre de pixels en hauteur 

#### Affichage
Si l'écran est initaliser avec _n_ pixel, alors les _n_ lignes qui suivent le registre d'initalisation sont reservé à la mémoire. 
C'est un écran monochrome, au moment de l'initalisation tout les pixels sont configurer en blanc. Il y a seulement les pixels donné en mémoire à la suite du registre d'initalisation seront de couleur noir.

Registre d'affichage :

<p align="center">
  <img src="./.img/screen_reg_aff.png" alt="screen_reg_aff">
</p>

- `x` : Position x du pixel à mettre en noir
- `y` : Position y du pixel à mettre en noir
  
Par exemple si on veut le pixel en _x=5_ et _y=5_ alors on doit mettre en mémoire: `0x00005005`



## screen_v0.py

Cette écran est le premier que nous avons réalisé, il a été conçus spécialement pour réaliser des jeux. Notament pour une bataille naval que l'on avait prévu de réaliser. \
A chaque modification de la mémoire, l'écran réalise un lecture des registres et affiche c'est modifications. 

Cette écran fonctionne, quand on on réalise une modification de la mémoire à la main et qu'on l'enregistre. Les modifications sont bien apporté sur l'écran. \
Contrairement avec l'ISS, ce problème vient de notre méthode de lecture et d'écriture dans la mémoire. Dans notre fichier `memory.cpp`, on ouvre une seul fois la mémoire, quand on écris dedans les modifications sont bien réalisé. Mais comme on ne le ferme jamais 
les modifications ne sont pas enregistrer, elle reste dans le buffer, alors l'écran ne peut pas détecter une modification. Il aurait fallu modifier notre méthode ; ouvrir et fermer la mémoire à chaque modification, mais par manque de temps nous n'avons pas pus réaliser c'est modifications.

<p align="center">
  <img src="./.img/screen_v0_1.png" alt="screenV0" width="350" height="350" >
  <img src="./.img/screen_v0_2.png" alt="screenV0" width="350" height="350">
</p>

#### Initialisation 
La première ligne de la mémoire, adresse `0x00000000`, permet d'initialiser l'écran.

Registre d'initialisation :

<p align="center">
  <img src="./.img/screen_v0_reg_init.png" alt="screen_v0_reg_init">
</p>

- `g` : Affichage d'une grille
- `f` : Facteur de taille, permet d'agrandir la taille d'un pixel
- `x` : Nombre de pixels en largeur
- `y` : Nombre de pixels en hauteur 

#### Affichage
Si l'écran est initaliser avec _n_ pixel, alors les _n_ lignes qui suivent le registre d'initalisation sont reservé à la mémoire. 
C'est un écran monochrome, au moment de l'initalisation tout les pixels sont configurer en blanc. Il y a seulement les pixels donné en mémoire à la suite du registre d'initalisation seront de couleur noir.

Registre d'affichage :

<p align="center">
  <img src="./.img/screen_v0_reg_aff.png" alt="screen_v0_reg_aff">
</p>

- `x` : Position x du pixel à mettre en couleur
- `y` : Position y du pixel à mettre en couleur
- `color` : Couleur du pixel en hexadécimal

Par exemple si on veut le pixel en _x=5_ et _y=5_ de couleur _gris_ alors on doit mettre en mémoire: `0x55E8E9EB`