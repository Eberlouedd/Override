# OverRide - Level00

## Analyse

En analysant le code source du programme, on remarque qu'une condition vérifie une valeur entrée par l'utilisateur.

Le programme attend le code suivant :

```text
5276
```

Si cette valeur est correcte, le programme lance un shell.

Une fois le shell obtenu, on peut lire le mot de passe du niveau suivant :

```bash
cat /home/users/level01/.pass
```

## Résultat

Le contenu du fichier correspond au mot de passe du niveau `level01`.