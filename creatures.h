/* J'ai créé ce fichier .h car en C on ne peut pas utiliser une structure
   dans plusieurs fichiers sans la déclarer quelque part en commun.
   C'est ici que je déclare toutes mes structures (Type, Espece, Individu, Parasite)
   et les prototypes de mes fonctions pour que main.c et creatures.c puissent les utiliser.*/

#ifndef CREATURES_H
#define CREATURES_H

#define NOM_MAX 50

// taille max de mon tableau d'individus
#define MAX_INDIVIDUS 500
#define MAX_PARASITES 50

// type de creature
#define TYPE_PREDATEUR 0
#define TYPE_HERBIVORE 1
#define TYPE_OISEAU    2
#define TYPE_POISSON   3
#define TYPE_PARASITE  4
#define TYPE_HERBE     5

// nombre d'espèces par type
#define NB_ESPECES_PREDATEUR 2
#define NB_ESPECES_HERBIVORE 2
#define NB_ESPECES_OISEAU    2
#define NB_ESPECES_POISSON   2
#define NB_ESPECES_PARASITE  2
#define NB_ESPECES_HERBE     1

// numéros des caractéristiques pour la reproduction
// j'ai fait un random de 1 a 3 pour choisir quelle caractéristique vient de quel parent et quel sera aléatoire
#define CAR_TAILLE     1
#define CAR_VITESSE    2
#define CAR_FORCE_FAIM 3


// un type regroupe plusieurs espèces
typedef struct {
    char nom[NOM_MAX];          
    int nb_individus_vivants;   
} Type;

// une espèce regroupe plusieurs individus
// les parametres de l'espèce sont les mêmes pour tous ses individus
typedef struct {
    char nom[NOM_MAX];          
    Type *type;                 // pointeur vers le type de cette espèce
    int nb_individus_vivants;   
    int sante;                  
    int intervalle_repro;       
    int valeur_nutritive;       // HP gagnés par celui qui mange un individu de cette espece
    int age_max;                
} Espece;


// un parasite a ses propres caractéristiques
// Au debut il infecte la créature la plus proche et lui fait des dégâts chaque tour
// IMPORTANT : il ne peut pas infecter un autre parasite ou une herbe
// Si la créature infectée est mangée, le parasite infecte qui l'a mangé.
// Si la créature infectée meurt de faim, le parasite meurt avec lui. 

typedef struct {
    Espece *espece;     // pointeur vers son espece
    int degats;         // HP enleves a la victime par tour
    int duree_vie;      
    int vivant;         
} Parasite;


// un individu
// taille, vitesse et force_faim sont propres a chaque individu
typedef struct {
    Espece *espece;            // pointeur vers son espèce
    int sante;
    int age;                   
    int x;                     
    int y;                     
    int tours_depuis_repro;    // nb de tours depuis la dernière reproduction
    int est_infecte;           
    Parasite *parasite;        // le parasite qui l'infecte (NULL si pas infecté)
    int vivant;                
    int taille;                
    int vitesse;               // affecte sur distance max parcourue par tour
    int force_faim;            // HP perdus par tour à cause de faim
} Individu;


// extern ça veut dire que ces variables existent dans creatures.c et que je peux les utiliser ici sans les recréer
extern Type type_predateur;
extern Type type_herbivore;
extern Type type_oiseau;
extern Type type_poisson;
extern Type type_parasite;
extern Type type_herbe;

// les espèces declarées dans creatures.c
extern Espece especes_predateurs[NB_ESPECES_PREDATEUR];
extern Espece especes_herbivores[NB_ESPECES_HERBIVORE];
extern Espece especes_oiseaux[NB_ESPECES_OISEAU];
extern Espece especes_poissons[NB_ESPECES_POISSON];
extern Espece especes_parasites[NB_ESPECES_PARASITE];
extern Espece especes_herbes[NB_ESPECES_HERBE];


// je declare ici les fonctions (que j'ai écrit dans creatures.c) pour que main.c puisse les utiliser

// initialise tous les types et espèces
void initialiser_types(void);
void initialiser_especes(void);

// fonction du random
int rand_entre(int min, int max);

// crée un parasite et un individu
Parasite *creer_parasite(Espece *espece);
Individu *creer_individu(Espece *espece, int x, int y);


void deplacer_individu(Individu *ind);

void mourir(Individu *ind);
void mourir_de_faim(Individu *ind);
void mourir_de_vieillesse(Individu *ind);

double distance(Individu *ind1, Individu *ind2);

void manger(Individu *ind1, Individu *ind2);


// le #ifndef au début et le #endif ici servent à éviter que ce fichier soit lu deux fois par le compilateur si plusieurs fichiers l'incluent
// ça évite les erreurs de "structure declaree deux fois"
#endif