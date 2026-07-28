/* J'ai décidé de ne pas écrire toute la logique concérnant les créatures dans main.c,
   car dans ce cas main.c va contenir des centaines de lignes mélangées:
   la gestion SDL, le menu, la simulation ET toute la logique des animaux,
   ce qui va devenir illisible. De plius ce serai plus simple d'ajouter des nouveaux animaux plus tard.
   J'ai donc créé ce fichier séparé où je mets uniquement tout ce qui concerne
   les créatures : leur création, leur initialisation. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
void initialiser_especes(void)
{
    // Prédateurs
    // grands, mais ont très faim
    strcpy(especes_predateurs[0].nom, "Tyrannosaurus");
    especes_predateurs[0].type = &type_predateur;
    especes_predateurs[0].nb_individus_vivants = 0;
    especes_predateurs[0].sante = 150;
    especes_predateurs[0].intervalle_repro = 10;
    especes_predateurs[0].valeur_nutritive = 80;
    especes_predateurs[0].age_max = 80;

    strcpy(especes_predateurs[1].nom, "Carnotaurus");
    especes_predateurs[1].type = &type_predateur;
    especes_predateurs[1].nb_individus_vivants = 0;
    especes_predateurs[1].sante = 140;
    especes_predateurs[1].intervalle_repro = 8;
    especes_predateurs[1].valeur_nutritive = 70;
    especes_predateurs[1].age_max = 60;

    // Herbivores
    strcpy(especes_herbivores[0].nom, "Triceratops");
    especes_herbivores[0].type = &type_herbivore;
    especes_herbivores[0].nb_individus_vivants = 0;
    especes_herbivores[0].sante = 130;
    especes_herbivores[0].intervalle_repro = 8;
    especes_herbivores[0].valeur_nutritive = 60;
    especes_herbivores[0].age_max = 70;

    strcpy(especes_herbivores[1].nom, "Stegosaurus");
    especes_herbivores[1].type = &type_herbivore;
    especes_herbivores[1].nb_individus_vivants = 0;
    especes_herbivores[1].sante = 120;
    especes_herbivores[1].intervalle_repro = 7;
    especes_herbivores[1].valeur_nutritive = 60;
    especes_herbivores[1].age_max = 60;

    // Oiseaux
    strcpy(especes_oiseaux[0].nom, "Quetzalcoatlus");
    especes_oiseaux[0].type = &type_oiseau;
    especes_oiseaux[0].nb_individus_vivants = 0;
    especes_oiseaux[0].sante = 100;
    especes_oiseaux[0].intervalle_repro = 8;
    especes_oiseaux[0].valeur_nutritive = 50;
    especes_oiseaux[0].age_max = 40;

    strcpy(especes_oiseaux[1].nom, "Pteranodon");
    especes_oiseaux[1].type = &type_oiseau;
    especes_oiseaux[1].nb_individus_vivants = 0;
    especes_oiseaux[1].sante = 90;
    especes_oiseaux[1].intervalle_repro = 7;
    especes_oiseaux[1].valeur_nutritive = 45;
    especes_oiseaux[1].age_max = 30;

    // Poissons
    strcpy(especes_poissons[0].nom, "Enchodus");
    especes_poissons[0].type = &type_poisson;
    especes_poissons[0].nb_individus_vivants = 0;
    especes_poissons[0].sante = 80;
    especes_poissons[0].intervalle_repro = 5;
    especes_poissons[0].valeur_nutritive = 40;
    especes_poissons[0].age_max = 20;

    strcpy(especes_poissons[1].nom, "Lepidotes");
    especes_poissons[1].type = &type_poisson;
    especes_poissons[1].nb_individus_vivants = 0;
    especes_poissons[1].sante = 60;
    especes_poissons[1].intervalle_repro = 4;
    especes_poissons[1].valeur_nutritive = 30;
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
    especes_herbes[0].sante = 40;
    especes_herbes[0].intervalle_repro = 14;
    especes_herbes[0].valeur_nutritive = 30;
    especes_herbes[0].age_max = 100;
}

// retourne un nombre aléatoire entre min et max inclus
int rand_entre(int min, int max)
{
    // si min > max on les echange
    if (min > max) {
        int tmp = min;
        min = max;
        max = tmp;
    }

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
        printf("Erreur malloc individu\n");
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
        ind->vitesse    = rand_entre(10, 20);
        ind->force_faim = rand_entre(3, 5);
    }
    else if (espece->type == &type_herbivore) {
        ind->taille     = rand_entre(10, 17);
        ind->vitesse    = rand_entre(8, 15);
        ind->force_faim = rand_entre(2, 4);
    }
    else if (espece->type == &type_oiseau) {
        ind->taille     = rand_entre(10, 17);
        ind->vitesse    = rand_entre(15, 25);
        ind->force_faim = rand_entre(2, 3);
    }
    else if (espece->type == &type_poisson) {
        ind->taille     = rand_entre(4, 7);
        ind->vitesse    = rand_entre(10, 18);
        ind->force_faim = rand_entre(1, 3);
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
        printf("Erreur malloc parasite\n");
        return NULL;
    }

    p->espece = espece;
    p->vivant = 1;
    p->x = rand_entre(10, 980);
    p->y = rand_entre(10, 780);
    p->a_infecte = 0;

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
    if (nouveau_x < 20)
        nouveau_x = 20;

    if (nouveau_x > 940)
        nouveau_x = 940;

    if (nouveau_y < 20)
        nouveau_y = 20;

    if (nouveau_y > 730)
        nouveau_y = 730;

    ind->x = nouveau_x;
    ind->y = nouveau_y;
}


void mourir_parasite(Parasite *p)
{
    if (p->vivant == 0) return;
    p->vivant = 0;
    p->a_infecte = 0;
    p->espece->nb_individus_vivants--;
    p->espece->type->nb_individus_vivants--;
}

void mourir(Individu *ind) {

    if (ind->vivant == 0) return;  // si déjà mort, on fait rien

    // si l'individu est infecté le parasite meurt avec lui
    if (ind->parasite != NULL)
        mourir_parasite(ind->parasite);

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


double distance(Individu *ind1, Individu *ind2){
    double dist = sqrt(pow((ind1->x - ind2->x),2) + pow((ind1->y - ind2->y),2)); //pow - mettre en puissance, sqrt - calculer la racine
    return dist;
}


// si la victime est infectée le parasite passe au mangeur
void transferer_parasite(Individu *mangeur, Individu *victime)
{
    if (victime->parasite != NULL) {
        mangeur->parasite = victime->parasite;
        mangeur->est_infecte = 1;
        victime->parasite = NULL;
        victime->est_infecte = 0;
    }
}


/*
chaîne alimentaire :
prédateur -> herbivore, oiseau (1 chance sur 2 que l'oiseau s'echappe en volant), prédateur plus petit
herbivore -> herbe
oiseau    -> poisson
poisson   -> herbe
parasite  -> prédateur, herbivore, oiseau, poisson (pas herbe ni parasite)

prédateur peut mourir à cause de faim ou vieillesse
parasites peuvent mourir à cause de mort de leur victime de faim, à cause de leur vieillesse
*/
int manger(Individu *ind1, Individu *ind2) {

    //j'utilise &type_predateur pour ne faire pas la comparaison chaque fois avec strcmp (strcmp(ind1->espece->type->nom, "Predateur") == 0)
    if (ind1->espece->type == &type_predateur) {
        if (ind2->espece->type == &type_predateur) {
            if (ind1->taille > ind2->taille) {

                transferer_parasite(ind1, ind2);

                ind1->sante += ind2->espece->valeur_nutritive;
                mourir(ind2);
                return 1;  // action reussie

            }
            else if (ind1->taille < ind2->taille) {

                transferer_parasite(ind2, ind1);

                ind2->sante += ind1->espece->valeur_nutritive;
                mourir(ind1);
                return 1;
            }
        }
        else if (ind2->espece->type == &type_herbivore) {

            transferer_parasite(ind1, ind2);

            ind1->sante += ind2->espece->valeur_nutritive;
            mourir(ind2);
            return 1;
        }
        else if (ind2->espece->type == &type_oiseau) {
            // Chance 1/2
            if (rand_entre(0, 1) == 0) {

                transferer_parasite(ind1, ind2);

                ind1->sante += ind2->espece->valeur_nutritive;
                mourir(ind2);
                return 1;
            }
        }
    }

    if (ind1->espece->type == &type_herbivore || ind1->espece->type == &type_poisson) {
        if (ind2->espece->type == &type_herbe) {
            ind1->sante += ind2->espece->valeur_nutritive;
            mourir(ind2);
            return 1;
        }
    }

    if (ind1->espece->type == &type_oiseau) {
        if (ind2->espece->type == &type_poisson) {

            transferer_parasite(ind1, ind2);

            ind1->sante += ind2->espece->valeur_nutritive;
            mourir(ind2);
            return 1;
        }
    }

    return 0; // rien ne s'est passe
}


int reproduire(Individu *ind1, Individu *ind2, Individu *individus[], int *nb_individus){
    if (ind1->espece == ind2->espece &&
        ind1->tours_depuis_repro > ind1->espece->intervalle_repro &&
        ind2->tours_depuis_repro > ind2->espece->intervalle_repro &&
        ind1->age >= 5 && ind2->age >= 5) {

        // on cree le bebe a la position à côté d'un parent
        Individu *ind_bebe = creer_individu(ind1->espece, rand_entre(10, 980), rand_entre(10, 780));

        // caractéristique du parent 1
        int c1 = rand_entre(1, 3);
        if (c1 == CAR_TAILLE){
            ind_bebe->taille = ind1->taille;
        }
        else if (c1 == CAR_VITESSE){
            ind_bebe->vitesse = ind1->vitesse;
        } 
        else{
            ind_bebe->force_faim = ind1->force_faim;
        }

        // caractéristique du parent 2
        int c2 = rand_entre(1, 3);
        while (c2 == c1){
            c2 = rand_entre(1, 3);
        }

        if (c2 == CAR_TAILLE){
            ind_bebe->taille = ind2->taille;
        }
        else if (c2 == CAR_VITESSE){
            ind_bebe->vitesse = ind2->vitesse;
        } 
        else{
            ind_bebe->force_faim = ind2->force_faim;
        }

        // troisème caractéristique aleatoire
        if ((c1 == CAR_TAILLE && c2 == CAR_VITESSE) || (c2 == CAR_TAILLE && c1 == CAR_VITESSE)){
            ind_bebe->force_faim = rand_entre(ind1->force_faim, ind2->force_faim);
        } 
        else if ((c1 == CAR_TAILLE && c2 == CAR_FORCE_FAIM ) || (c2 == CAR_TAILLE && c1 == CAR_FORCE_FAIM)){
            ind_bebe->vitesse = rand_entre(ind1->vitesse, ind2->vitesse);
        }
        else{
            ind_bebe->taille = rand_entre(ind1->taille, ind2->taille);
        }

        // j'ajoute le bebe dans le tableau
        if (*nb_individus < MAX_INDIVIDUS) {
            individus[*nb_individus] = ind_bebe;
            (*nb_individus)++;
        }

        // je remet les compteurs de reproduction a zero
        ind1->tours_depuis_repro = 0;
        ind2->tours_depuis_repro = 0;

        return 1;  // reproduction reussie
    }
    return 0;  // conditions pas remplies
}


// le parasite infecte l'individu (sauf l'autre parasite ou les herbes) le plus proche dans son rayon
void infecter(Parasite *p, Individu *individus[], int nb_individus)
{
    if (p->vivant == 0) return;

    if (p->a_infecte == 1) return;

    Individu *victime = NULL;
    double dist_min = 999999; // je prends pour premiére fois la distance très grande pour être sûre, qu'on aura le changement de valeur

    for (int i = 0; i < nb_individus; i++) {
        if (individus[i]->vivant == 0) continue; // mort
        if (individus[i]->parasite != NULL) continue;  // deja infecte
        if (individus[i]->espece->type == &type_herbe) continue;
        if (individus[i]->espece->type == &type_parasite) continue;

        double dist = sqrt(pow(p->x - individus[i]->x, 2) +
                           pow(p->y - individus[i]->y, 2)); // on ne peut pas utiliser la fonction dist, car on a Parasite comme paramétre et pas Individu

        if (dist < dist_min) {
            dist_min = dist;
            victime = individus[i];
        }
    }

    if (victime != NULL) {
        victime->parasite = p;
        victime->est_infecte = 1;
        p->a_infecte = 1;
    }
}


// le parasite reduit les HP de sa victime chaque tour
void attaquer_victime(Individu *ind)
{
    if (ind->parasite == NULL) return;
    if (ind->parasite->vivant == 0) return;

    ind->sante -= ind->parasite->degats;

    // duree de vie du parasite diminue
    ind->parasite->duree_vie--;

    // si la duree de vie est ecoulee le parasite meurt
    if (ind->parasite->duree_vie <= 0) {
        ind->parasite->vivant = 0;
        ind->parasite = NULL;
        ind->est_infecte = 0;
    }

    if (ind->sante <= 0) {
        mourir(ind);
    }
}