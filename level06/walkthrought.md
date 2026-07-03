## 1. Comprendre l'entrée attendue

Le code source attend un login et un serial.

## 2. Comprendre auth()

Dans la fonction `auth()`, qui attend le login et un int, la fonction fait des calculs à partir de la chaîne de caractères du login et les stocke dans une variable `v4`. Ensuite, la fonction `auth()` retourne 0 si le second paramètre fourni est égal à `v4`.

Si `auth` renvoie 0 dans `main()`, alors un shell est exécuté.

## 3. Reproduire le calcul

Nous allons faire un programme en C qui va reproduire les calculs que l'on fait sur la chaîne de caractères du login pour obtenir `v4`, et nous allons l'afficher.

## 4. Utiliser le résultat

Nous n'avons qu'à relancer `level06` avec un login et ce que nous avons obtenu avec le script dans `serial`.

```bash
cat /home/users/level07/.pass
```