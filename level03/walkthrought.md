## 1. Identifier les possibilités

Il n'y a que 21 possibilités à tester pour exécuter la fonction decrypt avec le bon paramètre, sinon il faudra trouver une autre solution.

## 2. Comprendre le fonctionnement de test()

Dans la fonction test(), il y a un switch avec la valeur a2 - a1.

test() est appelée dans main avec une entrée de l'utilisateur en premier argument et la valeur 322424845 en second argument.

```c
test(savedregs, 322424845);
```

## 3. Trouver le bon savedregs

Donc on va faire en sorte de donner le bon savedregs pour tomber sur les cases de la fonction test. On commence par 21.

```text
322424845 - 21 = 322424824 // ça ne marche pas
322424845 - 20 = 322424825 // ça ne marche pas
322424845 - 19 = 322424826 // ça ne marche pas
322424845 - 18 = 322424827 // ça marche, on a lancé un shell !
```
