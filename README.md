# Dinosaur Ecosystem Simulator

## Description
Simulateur d'évolution d'un écosystème de dinosaures en C avec SDL2.

## Fonctionnalités

### Menu principal
Au lancement, un menu permet de configurer la simulation et de choisir le nombre d'individus par espèce avant de démarrer.

### Écran de paramètres
L'utilisateur peut choisir combien d'individus de chaque espèce participent à la simulation. 
 Attention ! Il faut minimum 2 individus de la même espèce pour la reproduction.

### Simulation
- **Déplacement** : chaque individu se déplace aléatoirement selon sa vitesse
- **Faim** : chaque tour, un individu perd des HP. S'il mange, il en récupère
- **Chaîne alimentaire** : les prédateurs mangent les prédateurs plus petits, les herbivores et les oiseaux (chance 1/2, car ils peuvent voler), les oiseaux mangent les poissons, les herbivores et les poissons mangent l'herbe
- **Reproduction** : deux individus de la même espèce peuvent se reproduire s'ils sont assez proches et ont au moins 5 tours d'âge. L'enfant hérite d'une caractéristique de chaque parent (taille, vitesse ou force de faim), et la troisième est aléatoire. Après la reproduction, les parents doivent attendre un certain nombre de tours avant de pouvoir se reproduire à nouveau.
- **Parasites** : les parasites se déplacent et infectent l'animal le plus proche dans leur rayon d'action (sauf les herbes et autres parasites). Une fois qu'un animal est infecté, le parasite lui vole des HP chaque tour jusqu'à ce qu'il meure de vieillesse. Si l'animal infecté est mangé, le parasite passe automatiquement à celui qui l'a mangé.
- **Mort** : un individu meurt de faim, de vieillesse ou en étant mangé

### Contrôles
- **Start** : lance la simulation
- **Pause** : met en pause / reprend
- **Étape** : avance d'un seul tour (MAIS d'abord il faut appuier sur Pause)
- **Accélération** : accélère la simulation

### Panneau d'information
Liste des espèces vivantes triée par nombre d'individus décroissant.

## Choix techniques
J'ai décidé de séparer la logique des créatures dans `creatures.c` et `creatures.h` pour ne pas surcharger `main.c`. Les structures `Type`, `Espece`, `Individu` et `Parasite` sont manipulées via des tableaux de pointeurs.

## Commande pour une compilation
gcc main.c -o mon_prog $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lm