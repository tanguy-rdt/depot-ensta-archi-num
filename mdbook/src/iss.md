# ISS

## Utilisation 

- Depuis la racine du projet __sans__ `set-env`
   ```bash
   $ make -C simulateur
   $ ./simulateur/iss -i asm/data.asm -o bin/data.bin
   ```

- Depuis la racine du projet __avec__ `set-env`
   ```bash
   $ make -C simulateur
   $ iss -i asm/data.asm -o bin/data.bin
   ```


Il est nécessaire de spécifier le chemin du fichier binaire. Sans cette information le programme ne pourra pas s'executer.

## Gestion des Erreurs

Code de sortie | Information 
---------------|------------
0              | Programme executer avec succés
1              | Fichier binaire non précisé
2              | Erreur lors de l'ouverture du fichier binaire
3              | Dépassement du registre _(overflow)_