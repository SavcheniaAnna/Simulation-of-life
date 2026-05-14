/* J'ai décidé de ne pas écrire toute la logique concérnant les créatures dans main.c,
   car dans ce cas main.c va contenir des centaines de lignes mélangées:
   la gestion SDL, le menu, la simulation ET toute la logique des animaux,
   ce qui va devenir illisible. De plius ce serai plus simple d'ajouter des nouveaux animaux plus tard.
   J'ai donc créé ce fichier séparé où je mets uniquement tout ce qui concerne
   les créatures : leur création, leur initialisation. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creatures.h"

// je déclare mes types
Type type_predateur;
Type type_herbivore;
Type type_oiseau;
Type type_poisson;
Type type_parasite;
Type type_herbe;

// je déclare mes tableaux d'espèces
Espece especes_predateurs[NB_ESPECES_PREDATEUR];
Espece especes_herbivores[NB_ESPECES_HERBIVORE];
Espece especes_oiseaux[NB_ESPECES_OISEAU];
Espece especes_poissons[NB_ESPECES_POISSON];
Espece especes_parasites[NB_ESPECES_PARASITE];
Espece especes_herbes[NB_ESPECES_HERBE];

// j'initialise tous les types avec leur nom
void initialiser_types(void)
{
    strcpy(type_predateur.nom, "Predateur");
    type_predateur.nb_individus_vivants = 0;

    strcpy(type_herbivore.nom, "Herbivore");
    type_herbivore.nb_individus_vivants = 0;

    strcpy(type_oiseau.nom, "Oiseau");
    type_oiseau.nb_individus_vivants = 0;

    strcpy(type_poisson.nom, "Poisson");
    type_poisson.nb_individus_vivants = 0;

    strcpy(type_parasite.nom, "Parasite");
    type_parasite.nb_individus_vivants = 0;

    strcpy(type_herbe.nom, "Herbe");
    type_herbe.nb_individus_vivants = 0;
}

// j'initialise toutes les espèces avec leurs caractéristiques
/*
chaîne alimentaire :
prédateur -> herbivore, oiseau, prédateur plus petit
herbivore -> herbe
oiseau    -> poisson
poisson   -> herbe
parasite  -> prédateur, herbivore, oiseau, poisson (pas herbe ni parasite)

prédateur peut mourir à cause de faim ou vieillesse
parasites peuvent mourir à cause de mort de leur victime de faim, à cause de leur vieillesse
*/
void initialiser_especes(void)
{
    // Prédateurs
    // grands, mais ont très faim
    strcpy(especes_predateurs[0].nom, "Tyrannosaurus");
    especes_predateurs[0].type = &type_predateur;
    especes_predateurs[0].nb_individus_vivants = 0;
    especes_predateurs[0].sante = 100;
    especes_predateurs[0].intervalle_repro = 10;
    especes_predateurs[0].valeur_nutritive = 50;
    especes_predateurs[0].age_max = 80;

    strcpy(especes_predateurs[1].nom, "Carnotaurus");
    especes_predateurs[1].type = &type_predateur;
    especes_predateurs[1].nb_individus_vivants = 0;
    especes_predateurs[1].sante = 80;
    especes_predateurs[1].intervalle_repro = 8;
    especes_predateurs[1].valeur_nutritive = 40;
    especes_predateurs[1].age_max = 60;

    // Herbivores
    strcpy(especes_herbivores[0].nom, "Triceratops");
    especes_herbivores[0].type = &type_herbivore;
    especes_herbivores[0].nb_individus_vivants = 0;
    especes_herbivores[0].sante = 80;
    especes_herbivores[0].intervalle_repro = 8;
    especes_herbivores[0].valeur_nutritive = 35;
    especes_herbivores[0].age_max = 70;

    strcpy(especes_herbivores[1].nom, "Stegosaurus");
    especes_herbivores[1].type = &type_herbivore;
    especes_herbivores[1].nb_individus_vivants = 0;
    especes_herbivores[1].sante = 70;
    especes_herbivores[1].intervalle_repro = 7;
    especes_herbivores[1].valeur_nutritive = 30;
    especes_herbivores[1].age_max = 60;

    // Oiseaux
    strcpy(especes_oiseaux[0].nom, "Quetzalcoatlus");
    especes_oiseaux[0].type = &type_oiseau;
    especes_oiseaux[0].nb_individus_vivants = 0;
    especes_oiseaux[0].sante = 60;
    especes_oiseaux[0].intervalle_repro = 8;
    especes_oiseaux[0].valeur_nutritive = 30;
    especes_oiseaux[0].age_max = 40;

    strcpy(especes_oiseaux[1].nom, "Pteranodon");
    especes_oiseaux[1].type = &type_oiseau;
    especes_oiseaux[1].nb_individus_vivants = 0;
    especes_oiseaux[1].sante = 50;
    especes_oiseaux[1].intervalle_repro = 7;
    especes_oiseaux[1].valeur_nutritive = 25;
    especes_oiseaux[1].age_max = 30;

    // Poissons
    strcpy(especes_poissons[0].nom, "Enchodus");
    especes_poissons[0].type = &type_poisson;
    especes_poissons[0].nb_individus_vivants = 0;
    especes_poissons[0].sante = 30;
    especes_poissons[0].intervalle_repro = 5;
    especes_poissons[0].valeur_nutritive = 15;
    especes_poissons[0].age_max = 20;

    strcpy(especes_poissons[1].nom, "Lepidotes");
    especes_poissons[1].type = &type_poisson;
    especes_poissons[1].nb_individus_vivants = 0;
    especes_poissons[1].sante = 25;
    especes_poissons[1].intervalle_repro = 4;
    especes_poissons[1].valeur_nutritive = 12;
    especes_poissons[1].age_max = 15;

    // Parasites
    // vivent à l'intérieur du corps de leur victime et enlèvent des HP chaque tour
    // ne bougent pas, ne se reproduisent pas, ne peuvent pas infecter herbe ou parasite
    strcpy(especes_parasites[0].nom, "Helminth");
    especes_parasites[0].type = &type_parasite;
    especes_parasites[0].nb_individus_vivants = 0;

    strcpy(especes_parasites[1].nom, "Ascaridida");
    especes_parasites[1].type = &type_parasite;
    especes_parasites[1].nb_individus_vivants = 0;

    // Herbes
    // ne bougent pas, se reproduisent seules
    strcpy(especes_herbes[0].nom, "Fougere");
    especes_herbes[0].type = &type_herbe;
    especes_herbes[0].nb_individus_vivants = 0;
    especes_herbes[0].sante = 30;
    especes_herbes[0].intervalle_repro = 3;
    especes_herbes[0].valeur_nutritive = 20;
    especes_herbes[0].age_max = 999;
}

// retourne un nombre aléatoire entre min et max inclus
int rand_entre(int min, int max)
{
    // rand() génère un nombre aléatoire entre 0 et 32767
    // max - min + 1 : on calcule combien de nombres il y a entre max et min
    // % : le modulo, qui garde uniquement le reste de la division. Donc rand() % nb donne un nombre de 0 à (nb-1)
    return min + rand() % (max - min + 1);
}

// crée un individu d'une espèce donnée à une position x y
// taille, vitesse et force_faim sont aléatoires dans des limites selon le type
Individu *creer_individu(Espece *espece, int x, int y)
{
    Individu *ind = malloc(sizeof(Individu));

    if (ind == NULL) {
        fprintf(stderr, "Erreur malloc individu\n");
        return NULL;
    }

    ind->espece = espece;
    ind->sante = espece->sante;  // je copie la sante de l'espece
    ind->age = 0;
    ind->x = x;
    ind->y = y;
    ind->tours_depuis_repro = 0;
    ind->est_infecte = 0;
    ind->parasite = NULL;
    ind->vivant = 1;

    /*
    taille, vitesse et force_faim aléatoires selon le type
    prédateur : grand, moyen rapide, très faim
    herbivore : moyen, lent, faim moyenne
    oiseau    : grand (ptérosaure), très rapide, faim moyenne
    poisson   : petit, rapide, peu faim
    herbe     : ne bouge pas, pas faim
    */

    // je fais ça avec le rand pour avoir différents dinosaures au début
    // il faut avoir des différences pour la prochaine reproduction
    // je ne fais pas deux dinosaures de base, car dans les paramètres l'utilisateur a le droit de choisir combien de dinosaures de quelle espèce il veut avoir au début de la simulation
    if (espece->type == &type_predateur) {
        ind->taille     = rand_entre(15, 24);
        ind->vitesse    = rand_entre(4, 7);
        ind->force_faim = rand_entre(6, 9);
    }
    else if (espece->type == &type_herbivore) {
        ind->taille     = rand_entre(10, 17);
        ind->vitesse    = rand_entre(3, 6);
        ind->force_faim = rand_entre(4, 6);
    }
    else if (espece->type == &type_oiseau) {
        ind->taille     = rand_entre(10, 17);
        ind->vitesse    = rand_entre(7, 10);
        ind->force_faim = rand_entre(3, 4);
    }
    else if (espece->type == &type_poisson) {
        ind->taille     = rand_entre(4, 7);
        ind->vitesse    = rand_entre(5, 8);
        ind->force_faim = rand_entre(3, 5);
    }
    else if (espece->type == &type_herbe) {
        ind->taille     = rand_entre(3, 5);
        ind->vitesse    = 0;
        ind->force_faim = 0;
    }

    // j'incrémente les compteurs de l'espèce et du type
    espece->nb_individus_vivants++;
    espece->type->nb_individus_vivants++;

    return ind;
}

// crée un parasite d'une espèce donnée
// le parasite infecte la victime la plus proche au début
// IMPORTANT : ne peut pas infecter herbe ou parasite
Parasite *creer_parasite(Espece *espece)
{
    Parasite *p = malloc(sizeof(Parasite));

    if (p == NULL) {
        fprintf(stderr, "Erreur malloc parasite\n");
        return NULL;
    }

    p->espece = espece;
    p->vivant = 1;

    // Helminth : dégâts faibles mais dure longtemps
    if (strcmp(espece->nom, "Helminth") == 0) {
        p->degats = 3;
        p->duree_vie = 50;
    }
    // Dicopoelium : dégâts plus forts mais dure moins longtemps
    else if (strcmp(espece->nom, "Ascaridida") == 0) {
        p->degats = 6;
        p->duree_vie = 30;
    }

    espece->nb_individus_vivants++;
    espece->type->nb_individus_vivants++;

    return p;
}

void deplacer_individu(Individu *ind)
{
    // l'herbe ne bouge pas
    if (ind->vitesse == 0)
        return;

    // direction aleatoire entre -vitesse et +vitesse
    int dx = rand_entre(-ind->vitesse, ind->vitesse);
    int dy = rand_entre(-ind->vitesse, ind->vitesse);

    // nouvelle position
    int nouveau_x = ind->x + dx;
    int nouveau_y = ind->y + dy;

    // on verifie les limites de la zone de simulation
    if (nouveau_x < 10)
        nouveau_x = 10;

    if (nouveau_x > 980)
        nouveau_x = 980;

    if (nouveau_y < 10)
        nouveau_y = 10;

    if (nouveau_y > 780)
        nouveau_y = 780;

    ind->x = nouveau_x;
    ind->y = nouveau_y;
}

void mourir(Individu *ind) {
    ind->vivant = 0;
    ind->espece->nb_individus_vivants--;
    ind->espece->type->nb_individus_vivants--;
}

void mourir_de_faim(Individu *ind)
{
    ind->sante -= ind->force_faim;  // sante de l'individu pas de l'espece !
    if (ind->sante <= 0) {
        mourir(ind);
    }
}

void mourir_de_vieillesse(Individu *ind)
{
    ind->age++;
    if (ind->age >= ind->espece->age_max) {
        mourir(ind);
    }
}