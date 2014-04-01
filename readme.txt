En l'état actuel, l'analyseur lexical (xml.l) est complet et ne devrait
pas nécessiter d'être modifié. Pour ce qui est de l'analyseur syntaxique
(xml.y), il ne reconnait que les flux avec des éléments qui peuvent contenir
d'autres éléments, sans espace de noms, sans attribut, sans commentaires,
sans texte ni section CDATA, sans en-têtes. Il faut donc compléter cette 
grammaire.

La lecture actuelle est basée sur le flux standard stdin, pour changer cela
il faudra utiliser la variable xmlin de type FILE *. Pour ouvrir un 
fichier il faudra recourrir à la fonction fopen().
