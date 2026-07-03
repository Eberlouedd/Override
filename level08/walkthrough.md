## 1. Comprendre le comportement

On a cette fois un binaire et un dossier `backups`.

On se rend compte que, dans le code source, l'argument du programme sera considéré comme un fichier, il sera ouvert, puis le contenu du fichier sera copié dans `backups` + le chemin du fichier ouvert.

Donc, si on fait `./level8 /home/users/level09/.pass`, ça ne marchera pas car `./backups//home/users/level09/.pass` n'existe pas à cause des doubles slashs.

Et si on ne met pas le slash, le chemin `home/users/level09/.pass` sans le slash n'existe pas.

## 2. Passer par un lien symbolique

On va donc passer par un lien symbolique.

```bash
chmod 777 .
ln -s /home/users/level09/.pass exploit
./level exploit
cat backups/exploit
```
