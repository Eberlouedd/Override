Il n y a 21 possibilite a teste pour execter la fonction decrypt avec le bon parametre, sinon il faudra trouver une autre solution

dans la fonction test() il y a un switch avec la valeur a2 - a1

test() est appele dans main avec une entre de lutilisateur en premier argument et la valeur 322424845 en second argument

test(savedregs, 322424845);

donc on va faire en sorte de donner le bon savedregs pour tomber sur les case de la fonction test, on commence par 21

322424845 - 21 = 322424824 // ca ne marche pas
322424845 - 20 = 322424825 // ca ne marche pas
322424845 - 19 = 322424826 // ca ne marche pas
322424845 - 18 = 322424827 // ca marche, on a lance un shell !
