On a cette fois un binaire et un dossier "backups".
on se rend compte que dans le code source que l'argument du programme sera considerer comme un fichier et sera ouvert
et le contenu du fichier sera copier dans backups + chemin du fichier ouvert
Donc si on fait ./level8 /home/users/level09/.pass ca ne marchera pas car ./backups//home/users/level09/.pass n'existe pas a cause des double slash
et son on ne met pas le slash bah le chemin home/users/level09/.pass sans le slash n'existe pas
on va donc passer par un lien symbolique

chmod 777 .
ln -s /home/users/level09/.pass exploit
./level exploit
cat backups/exploit
