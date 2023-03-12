# Screen 

Notre écran s'utilise à l'aide de l'appel system "_scall 5_". \
Pour pouvoir l'utiliser il est nécessaire d'avoir _PySide6_ :

```bash
$ pip install pyside6
```

## screen.py
Cette écran à était conçus principalement pour réaliser des dessins ou des graphiques, il n'est pas réellement adapter pour afficher une image monochrome.



#### Initialisation 
La première ligne de la mémoire, adresse `0x00000000`, permet d'initialiser l'écran.

Registre d'initialisation :

![](./.img/screen_reg_init.png)

#### Affichage
Si l'écran est initaliser avec _n_ pixel, alors les _n_ lignes qui suivent le registre d'initalisation sont reservé à la mémoire. 
C'est un écran monochrome, au moment de l'initalisation tout les pixels sont configurer en blanc. Il y a seulement les pixels donné en mémoire à la suite du registre d'initalisation seront de couleur noir.

Registre d'affichage :

![](./.img/screen_reg_aff.png)

Par exemple si on veut le pixel en _x=5_ et _y=5_ alors on doit mettre en mémoire: `0x00005005`



## screen_v0.py

Cette écran est le premier que nous avons réalisé, il a été conçus spécialement pour réaliser des jeux. Notament pour une bataille naval que l'on avait prévu de réaliser. \
A chaque modification de la mémoire, l'écran réalise un lecture des registres et affiche c'est modifications. 

Cette écran fonctionne, quand on on réalise une modification de la mémoire à la main et qu'on l'enregistre. Les modifications sont bien apporté sur l'écran. \
Contrairement avec l'ISS, ce problème vient de notre méthode de lecture et d'écriture dans la mémoire. Dans notre fichier `memory.cpp`, on ouvre une seul fois la mémoire, quand on écris dedans les modifications sont bien réalisé. Mais comme on ne le ferme jamais 
les modifications ne sont pas enregistrer, elle reste dans le buffer, alors l'écran ne peut pas détecter une modification. Il aurait fallu mmodifier notre méthode ; ouvrir et fermer la mémoire à chaque modification, mais par manque de temps nous n'avons pas pus réaliser c'est modifications.

#### Initialisation 
La première ligne de la mémoire, adresse `0x00000000`, permet d'initialiser l'écran.

Registre d'initialisation :

![](./.img/screen_reg_init.png)

#### Affichage
Si l'écran est initaliser avec _n_ pixel, alors les _n_ lignes qui suivent le registre d'initalisation sont reservé à la mémoire. 
C'est un écran monochrome, au moment de l'initalisation tout les pixels sont configurer en blanc. Il y a seulement les pixels donné en mémoire à la suite du registre d'initalisation seront de couleur noir.

Registre d'affichage :

![](./.img/screen_reg_aff.png)

Par exemple si on veut le pixel en _x=5_ et _y=5_ alors on doit mettre en mémoire: `0x00005005`