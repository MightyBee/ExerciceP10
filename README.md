-Le projet a été fait en entier. Nous avons fait tous les problèmes donnés de manière hebdommadaire.

-Oui, nous avons une version graphique avec la bibliothèque Qt WxWidget.

-Nous estimons que le temps consacré individuellement au projet est de 3h par semaine.

Description du programme:
    Notre programme permet la simulation de différents oscillateurs, plus précisemment de pendules,
 ressorts, pendules-doubles, pendules à torsion, chariots(qui sont des pendules reliés à des
 ressorts) et de pendules-ressorts(qui sont des pendules où la tige est un ressort). Les différents
 paramètres de ces oscillateurs (masse, longueur, constante de frottement etc) peuvent être
 modifiés et la simulation agit en conséquence. Quelques extensions ont été rajoutés au programme :
 1 : En appuyant sur la touche "v" du clavier, les oscillateurs deviennent des sphères (alors que
 leur configuration de base est en cube) et la couleur des oscillateurs change en fonction de leur
 vitesse. En effet, plus un oscillateur a une vitesse élevée, plus il est rouge. De manière
 réciproque plus un oscillateur à une basse vitesse, plus il est colorié en blanc.
 2 : En appuyant sur la touche "o" du clavier, tous les graphiques de l'espace des phases de chaque
 oscillateur apparaissent. Pour fermer ces graphiques il suffit d'appuyer sur la touche "0".
 Si vous voulez cibler un oscillateur et ne visionner que son espace de phase il faut appuyer sur sa
 touche correspondante : la touche "1" cible le premier oscillateur, la touche "2" le deuxième etc.

 Deux modes de simulation sont possibles: le mode texte et le mode graphique.

 Pour utiliser le mode graphique, il faut:
      1) aller dans le fichier main_qt_gl.cc
      2) dans le main initialiser les oscillateurs voulus en consultant les différents constructeurs
      3) les ajouter avec la méthode add (avec la sytaxe w.add(Oscillateur_voulu);)

Pour compiler le mode graphique il faut:
      1) aller dans le dossier Qt_GL et exécuter qmake puis make

//IL ME SEMBLE QUE C'EST COMME CA POUR GRAPHIQUE

Pour utiliser le mode texte, il faut:
      //AUCUNE IDEE J'UTILISE PAS LE MODE TEXTE
Pour compiler le mode texte, il faut:


 Nous sommes fiers d'être parvenus au bout du projet, qui était conséquent pour nous. La partie
 graphique et l'espace des phases est la plus grande source de satisfaction et de fierté de ce
 projet.
