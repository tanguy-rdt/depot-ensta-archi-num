# Utilisation générale


Nous avons décidé de garder l'asssembleur et l'iss dans deux dossier séparé pour garder une structure de fichier cohérente. Ce choix peut être pénalisant pour leurs utilisations, ce déplacer dans différent dossier ou encore donner le chemin relatif au moment de l'éxecution peut devenir pénible.

C'est pourquoi vous avez la possibilité d'éxecuter le script `set-env.sh`, pour éxecuter l'iss et l'assembleur depuis la racine du projet ce qui facilitera la manipulation des fichier _.asm_ et _.bin_.

```bash
$ source set-env.sh
$ iss # éxecution de l'iss
$ assembleur # éxecution de l'assembleur
```

Le script ajoute simplement les exécutables sous forme d'alias :
```bash
alias assembleur="python3 ${CURRENT_DIR}/assembleur/assembleur.py"
alias iss="${CURRENT_DIR}/simulateur/iss"
```

---

Après avoir utiliser notre projet vous pouvez retirer c'est alias si vous le souhaitez, au lieu de fermer et ouvrir un nouveau terminal :

```bash
$ source unset-env.sh
```