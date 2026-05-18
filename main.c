#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "creatures.h"
#include <stdlib.h>
#include <time.h>

// Taille de la fenetre
#define LARGEUR 1400
#define HAUTEUR 800

//Pour ouvrir différentes fenêtres en appuiant sur les boutons
#define ETAT_MENU        0
#define ETAT_PARAMETRES  1
#define ETAT_SIMULATION  2

int main(void) {

/* 1. Initialisation SDL*/
if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
    return 1;
}

// Initialiser SDL_image
if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());
}

// Initialiser SDL_ttf
if (TTF_Init() != 0) {
fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
}


/* 2. Creation de la fenetre et du renderer */
SDL_Window *win = SDL_CreateWindow(
    "Simulateur d'evolution - Dinosaures", // titre
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // position
    LARGEUR, HAUTEUR, // largeur x hauteur
    0 // flags (0 = rien de special)
);

SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


// initialisation du générateur aleatoire
srand(time(NULL));

// initialisation des types et espèces
initialiser_types();
initialiser_especes();

// tableaux d'individus et de parasites
Individu *individus[MAX_INDIVIDUS];
Parasite *parasites[MAX_PARASITES];
int nb_individus = 0;
int nb_parasites = 0;


// Charger l’image dans une texture GPU
// J'ai écrit un prompt détaillé pour le chat gpt, pour qu'il génère une image de taille 1400 x 800, pour illustrer mon projet
SDL_Texture *fond_menu = IMG_LoadTexture(ren, "assets/fond_menu.png");
SDL_Texture *fond_parametres = IMG_LoadTexture(ren, "assets/fond_parametres.png");
if (!fond_menu || !fond_parametres) {
    fprintf(stderr, "IMG_LoadTexture: %s\n", IMG_GetError());
}

// Definir ou la placer (NULL = toute la fenetre)
SDL_Rect dst = {0, 0, LARGEUR, HAUTEUR};


// Charger une police (chemin, taille en points)
// J'ai décidé que je veux utiliser un beaux font, donc je l'ai choisi et charger sur le site google fonts
TTF_Font *font = TTF_OpenFont("assets/Exo2-Medium.ttf", 15);
if (!font) {
    fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
}

// Texte pour le Menu
SDL_Color blanc_casse = {245, 245, 220, 255};
char *lignes[] = {
    "Dans cette simulation, chaque animal perd des HP de faim à chaque tour.",
    "S'il mange, il en récupère. À chaque tour, un dinosaure fait soit manger soit",
    "se reproduire. Les prédateurs mangent les herbivores, les oiseaux et les",
    "prédateurs d'une autre espèce plus petits. Les oiseaux mangent les poissons.",
    "Les herbivores et les poissons mangent l'herbe. Les parasites infectent les",
    "animaux et leur volent des HP chaque tour — si l'animal infecté est mangé,",
    "le parasite passe au prédateur !",
    "Avant de lancer la simulation, choisissez les espèces qui participent et",
    "combien d'individus de chaque espèce vous voulez."
};
int nb_lignes = 9;


int etat = ETAT_MENU;  // on est dans le menu

// Boutons Démarrer et Paramètres
SDL_Rect bouton_start = {740, 575, 110, 35}; // x, y, largeur, hauteur
SDL_Surface *surf_btn = TTF_RenderUTF8_Blended(font, "Démarrer", blanc_casse);
SDL_Texture *texte_btn = SDL_CreateTextureFromSurface(ren, surf_btn);
SDL_FreeSurface(surf_btn);
int wb, hb;
SDL_QueryTexture(texte_btn, NULL, NULL, &wb, &hb);

SDL_Rect bouton_par = {540, 575, 110, 35};
SDL_Surface *surf_btn_par = TTF_RenderUTF8_Blended(font, "Parametres", blanc_casse);
SDL_Texture *texte_btn_par = SDL_CreateTextureFromSurface(ren, surf_btn_par);
SDL_FreeSurface(surf_btn_par);
int wbp, hbp;
SDL_QueryTexture(texte_btn_par, NULL, NULL, &wbp, &hbp);



// Les eléments pour le fenêtre Paramètres

//Bouton Revenir
SDL_Rect bouton_rev = {50, 30, 110, 35};
SDL_Surface *surf_btn_rev = TTF_RenderUTF8_Blended(font, "Revenir", blanc_casse);
SDL_Texture *texte_btn_rev = SDL_CreateTextureFromSurface(ren, surf_btn_rev);
SDL_FreeSurface(surf_btn_rev);
int wbr, hbr;
SDL_QueryTexture(texte_btn_rev, NULL, NULL, &wbr, &hbr);


char *lignes_par[] = {
    "PREDATEURS",
    "  Tyrannosaurus",
    "  Carnotaurus",
    
    "HERBIVORES",
    "  Triceratops",
    "  Stegosaurus",
    
    "OISEAUX",
    "  Quetzalcoatlus",
    "  Pteranodon",
    
    "POISSONS",
    "  Enchodus",
    "  Lepidotes",
    
    "PARASITES",
    "  Helminth",
    "  Dicopoelium",
    
    "HERBE",
    "  Fougere"
};
int nb_lignes_par = 17;

// valeurs choisies par l'utilisateur et le max 
int nb_par_espece[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int max_par_espece[11] = {10, 10, 10, 10, 10, 10, 10, 10, 5, 5, 20};

int case_active = -1; // nombre de rectangles sur le quel on a appuié
char saisie[3] = ""; //pour stocker ce que l'utilisateur a entré (3 parce que on a max 20, donc ce serait "20\0")


// Bouton Sauvegarder
SDL_Rect bouton_sauv = {620, 680, 130, 35};
SDL_Surface *surf_btn_sauv = TTF_RenderUTF8_Blended(font, "Sauvegarder", blanc_casse);
SDL_Texture *texte_btn_sauv = SDL_CreateTextureFromSurface(ren, surf_btn_sauv);
SDL_FreeSurface(surf_btn_sauv);
int wbs, hbs;
SDL_QueryTexture(texte_btn_sauv, NULL, NULL, &wbs, &hbs);



// Les eléments pour le fenêtre Simulation

//Bouton Start
SDL_Rect bouton_sim_start = {1030, 685, 100, 35};
SDL_Surface *surf_sim_start = TTF_RenderUTF8_Blended(font, "Start", blanc_casse);
SDL_Texture *texte_sim_start = SDL_CreateTextureFromSurface(ren, surf_sim_start);
SDL_FreeSurface(surf_sim_start);
int wss, hss;
SDL_QueryTexture(texte_sim_start, NULL, NULL, &wss, &hss);


// timer pour controler la vitesse de la simulation
Uint32 dernier_tour = SDL_GetTicks();
int delai_tour = 200;  // un tour toutes les 200ms


// bouton Pause
SDL_Rect bouton_sim_pause = {1150, 685, 100, 35};
SDL_Surface *surf_sim_pause = TTF_RenderUTF8_Blended(font, "Pause", blanc_casse);
SDL_Texture *texte_sim_pause = SDL_CreateTextureFromSurface(ren, surf_sim_pause);
SDL_FreeSurface(surf_sim_pause);
int wsp, hsp;
SDL_QueryTexture(texte_sim_pause, NULL, NULL, &wsp, &hsp);

// simulation en cours ou en pause
int simulation_active = 0;


// bouton Etape
SDL_Rect bouton_sim_etape = {1270, 685, 100, 35};
SDL_Surface *surf_sim_etape = TTF_RenderUTF8_Blended(font, "Etape", blanc_casse);
SDL_Texture *texte_sim_etape = SDL_CreateTextureFromSurface(ren, surf_sim_etape);
SDL_FreeSurface(surf_sim_etape);
int wse, hse;
SDL_QueryTexture(texte_sim_etape, NULL, NULL, &wse, &hse);


// bouton Accélération
SDL_Rect bouton_sim_accel = {1150, 740, 110, 35};
SDL_Surface *surf_sim_accel = TTF_RenderUTF8_Blended(font, "Accélération", blanc_casse);
SDL_Texture *texte_sim_accel = SDL_CreateTextureFromSurface(ren, surf_sim_accel);
SDL_FreeSurface(surf_sim_accel);
int wsa, hsa;
SDL_QueryTexture(texte_sim_accel, NULL, NULL, &wsa, &hsa);

// on vérifie si on a accélération
int accel_active = 0;



/* 3. Boucle principale */
int running = 1;
SDL_Event event;
while (running) {

    /* Gestion des evenements */
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = 0;
        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == SDLK_ESCAPE)
            running = 0;

        // Je vérifie si l'utilisateur a appuié sur l'un des boutons
        if (event.type == SDL_MOUSEBUTTONDOWN) {

            if (etat == ETAT_MENU) {
                // clic sur bouton Paramètres
                if (event.button.x >= bouton_par.x &&
                    event.button.x <= bouton_par.x + bouton_par.w &&
                    event.button.y >= bouton_par.y &&
                    event.button.y <= bouton_par.y + bouton_par.h)
                    etat = ETAT_PARAMETRES;
                // clic sur bouton Démarrer
                if (event.button.x >= bouton_start.x &&
                    event.button.x <= bouton_start.x + bouton_start.w &&
                    event.button.y >= bouton_start.y &&
                    event.button.y <= bouton_start.y + bouton_start.h)
                    etat = ETAT_SIMULATION;
            }
            else if (etat == ETAT_PARAMETRES) {

                // clic sur bouton Revenir
                if (event.button.x >= bouton_rev.x &&
                    event.button.x <= bouton_rev.x + bouton_rev.w &&
                    event.button.y >= bouton_rev.y &&
                    event.button.y <= bouton_rev.y + bouton_rev.h) {
                        case_active = -1;
                        saisie[0] = '\0';
                        etat = ETAT_MENU;
                }

                // clic sur bouton Sauvegarder
                if (event.button.x >= bouton_sauv.x &&
                    event.button.x <= bouton_sauv.x + bouton_sauv.w &&
                    event.button.y >= bouton_sauv.y &&
                    event.button.y <= bouton_sauv.y + bouton_sauv.h) {
                    case_active = -1;
                    saisie[0] = '\0';
                    etat = ETAT_MENU;
                }

                // clic sur rectangles
                int index_case = 0;
                for (int i = 0; i < nb_lignes_par; i++) {
                    if (lignes_par[i][0] == ' ') {
                        SDL_Rect case_rect = {760, 140 + i * 30, 35, 20};
                        if (event.button.x >= case_rect.x &&
                            event.button.x <= case_rect.x + case_rect.w &&
                            event.button.y >= case_rect.y &&
                            event.button.y <= case_rect.y + case_rect.h) {

                            case_active = index_case; // on sauvegarder l'indice de case, donc l'indice de l'animal
                            saisie[0] = '\0';
                        }

                        index_case++;
                    }
                }
            }
            else if (etat == ETAT_SIMULATION){
                // clic Start
                if (event.button.x >= bouton_sim_start.x &&
                    event.button.x <= bouton_sim_start.x + bouton_sim_start.w &&
                    event.button.y >= bouton_sim_start.y &&
                    event.button.y <= bouton_sim_start.y + bouton_sim_start.h) {
            
                    // initialiser les individus, sachant les valeurs des parametres
                    nb_individus = 0;

                    // On a besoin de créer ce tableu, car on passe une espèce en paramètre à la fonction creer_individu, mais on sait pas quelle espèce correspond à quel index de nb_par_espece
                    Espece *toutes_especes[11] = {
                        &especes_predateurs[0], &especes_predateurs[1],
                        &especes_herbivores[0], &especes_herbivores[1],
                        &especes_oiseaux[0],    &especes_oiseaux[1],
                        &especes_poissons[0],   &especes_poissons[1],
                        &especes_parasites[0],  &especes_parasites[1],
                        &especes_herbes[0]
                    };

                    for (int e = 0; e < 11; e++) {
                        for (int i = 0; i < nb_par_espece[e]; i++) {
                            if (e == 8 || e == 9){
                                parasites[nb_parasites++] = creer_parasite(toutes_especes[e]);
                            }
                            else{
                                individus[nb_individus++] = creer_individu(toutes_especes[e], rand_entre(20, 980), rand_entre(20, 780)); // position random
                            }
                        }
                    }

                    simulation_active = 1;
                }

                // clic Pause
                if (event.button.x >= bouton_sim_pause.x &&
                    event.button.x <= bouton_sim_pause.x + bouton_sim_pause.w &&
                    event.button.y >= bouton_sim_pause.y &&
                    event.button.y <= bouton_sim_pause.y + bouton_sim_pause.h){

                    simulation_active = !simulation_active; // on met en pause ou on continue la simulation
                }

                // clic Etape
                if (simulation_active == 0) {
                    if (event.button.x >= bouton_sim_etape.x &&
                        event.button.x <= bouton_sim_etape.x + bouton_sim_etape.w &&
                        event.button.y >= bouton_sim_etape.y &&
                        event.button.y <= bouton_sim_etape.y + bouton_sim_etape.h) {
                        for (int i = 0; i < nb_individus; i++) {
                            if (individus[i]->vivant == 1)
                                deplacer_individu(individus[i]);
                                mourir_de_faim(individus[i]);
                                mourir_de_vieillesse(individus[i]);
                                individus[i]->tours_depuis_repro++;
                        }
                    }
                }

                //clic Accélération
                if (event.button.x >= bouton_sim_accel.x &&
                    event.button.x <= bouton_sim_accel.x + bouton_sim_accel.w &&
                    event.button.y >= bouton_sim_accel.y &&
                    event.button.y <= bouton_sim_accel.y + bouton_sim_accel.h) {
                    accel_active = !accel_active;
                    if (accel_active == 1){
                        delai_tour = 50;   // plus vite
                    }
                    else{
                        delai_tour = 200;  // vitesse normale
                    }
                }
            }
        }

        // (quand case est active) on entre le valeur et le sauvgarde
        if (etat == ETAT_PARAMETRES && case_active != -1) {
            if (event.type == SDL_KEYDOWN) {

                // On appuie sur Entrée (pour confirmer la valeur)
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (saisie[0] != '\0') {
                        int val = atoi(saisie); // je convrtis une chaîne de caractères en nombre

                        if (val < 0){
                            val = 0;
                        }

                        if (val > max_par_espece[case_active]){
                            val = max_par_espece[case_active];
                        }

                        nb_par_espece[case_active] = val; // on enregistre le valeur
                    }
                    case_active = -1;
                    saisie[0] = '\0';
                }

                // On appuie sur Backspace
                else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int len = strlen(saisie);
                    if (len > 0)
                        saisie[len - 1] = '\0'; // je supprime le derniére caractère
                }

                // On appuie sur les chiffres
                else if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
                    int len = strlen(saisie); 
                    if (len < 2) { // maximum 2 chiffres dans le nombre
                        saisie[len] = (char)event.key.keysym.sym; // on récupère le chiffre, le quel on a entré
                        saisie[len + 1] = '\0';
                    }
                }
            }
        }
    }
        
    
    if (etat == ETAT_MENU) {

        //Afficher le fond_menu
        SDL_RenderCopy(ren, fond_menu, NULL, &dst);


        // Afficher chaque ligne de texte
        for (int i = 0; i < nb_lignes; i++) {
            if (lignes[i][0] == '\0') {  // si on a une ligne vide, on saute
            continue;
            }
            SDL_Surface *surf = TTF_RenderUTF8_Blended(font, lignes[i], blanc_casse);
            SDL_Texture *texte = SDL_CreateTextureFromSurface(ren, surf);
            SDL_FreeSurface(surf);      // la surface n'est plus utile

            int w, h;
            SDL_QueryTexture(texte, NULL, NULL, &w, &h);
            SDL_Rect dst_texte = {437, 310 + i * 25, w, h};  // 25 pixels entre chaque ligne
            SDL_RenderCopy(ren, texte, NULL, &dst_texte);
            SDL_DestroyTexture(texte);
        }

        
        //Paramètres de bouton Démarrer
        SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
        SDL_RenderFillRect(ren, &bouton_start);
        // contour
        SDL_SetRenderDrawColor(ren, 140, 125, 90, 255);
        SDL_RenderDrawRect(ren, &bouton_start);

        SDL_Rect dst_btn = {bouton_start.x + (bouton_start.w - wb) / 2, bouton_start.y + (bouton_start.h - hb) / 2, wb, hb};
        SDL_RenderCopy(ren, texte_btn, NULL, &dst_btn);


        // Paramètres de bouton Paramètres
        SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
        SDL_RenderFillRect(ren, &bouton_par);
        // contour blanc-casse
        SDL_SetRenderDrawColor(ren, 140, 125, 90, 255);
        SDL_RenderDrawRect(ren, &bouton_par);
        
        SDL_Rect dst_btn_par = {bouton_par.x + (bouton_par.w - wbp) / 2, bouton_par.y + (bouton_par.h - hbp) / 2, wbp, hbp};
        SDL_RenderCopy(ren, texte_btn_par, NULL, &dst_btn_par);
    }


    else if (etat == ETAT_PARAMETRES) {
        
        //Afficher le fond_parametres
        SDL_RenderCopy(ren, fond_parametres, NULL, &dst);

        //Afficher le texte en haut
        SDL_Surface *surf = TTF_RenderUTF8_Blended(font, "Il faut minimum 2 individus de la même espèce pour qu'ils puissent se reproduire !", blanc_casse);
        SDL_Texture *texte = SDL_CreateTextureFromSurface(ren, surf);
        SDL_FreeSurface(surf);

        int w, h;
        SDL_QueryTexture(texte, NULL, NULL, &w, &h);
        SDL_Rect dst_texte = {420, 110, w, h};
        SDL_RenderCopy(ren, texte, NULL, &dst_texte);
        SDL_DestroyTexture(texte);


        // afficher toutes les animaux
        int index_case = 0;
        for (int i = 0; i < nb_lignes_par; i++) {

            if (lignes_par[i][0] == '\0'){ // si on a une ligne vide, on saute
                continue;
            }

            SDL_Surface *surf_list = TTF_RenderUTF8_Blended(font, lignes_par[i], blanc_casse);
            SDL_Texture *texte_list = SDL_CreateTextureFromSurface(ren, surf_list);
            SDL_FreeSurface(surf_list);
            int w, h;
            SDL_QueryTexture(texte_list, NULL, NULL, &w, &h);
            SDL_Rect dst_texte_list = {560, 140 + i * 30, w, h};
            SDL_RenderCopy(ren, texte_list, NULL, &dst_texte_list);
            SDL_DestroyTexture(texte_list);

            // dans les lignes, les quelles comencent par l'espace, on déssine un petit carré, dans le quel on vas entrer le nombre des individus
            if (lignes_par[i][0] == ' ') {

                SDL_Rect case_rect = {760, 140 + i * 30, 35, 20};

                // Je change le couleur d'une case, si elle active
                if (case_active == index_case){
                    SDL_SetRenderDrawColor(ren, 140, 125, 90, 255);
                }
                else{
                    SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
                }

                SDL_RenderFillRect(ren, &case_rect);
                SDL_SetRenderDrawColor(ren, 245, 245, 220, 255);
                SDL_RenderDrawRect(ren, &case_rect);


                char valeur[3]; // valeur dans la case
                // j'utilise snprintf pour convertir un entier en chaine de caracteres, les paramètres sont (où, taille, format, quoi)
                snprintf(valeur, sizeof(valeur), "%d", nb_par_espece[index_case]);


                SDL_Surface *surf_val_nb;
                if (case_active == index_case && saisie[0] != '\0'){ // si la case est active et l'utilisateur est en train de taper
                    surf_val_nb = TTF_RenderUTF8_Blended(font, saisie, blanc_casse); // j'affiche ce qu'il tape en ce moment
                }
                else {
                    // Si la case n'est pas active, le tableau saisie est vide. C'est pour ça qu'on ne voit plus le nombre après l'appui sur Entrée.
                    // donc ici j'affiche la valeur sauvegardée
                    surf_val_nb = TTF_RenderUTF8_Blended(font, valeur, blanc_casse);
                }

                SDL_Texture *texte_val_nb = SDL_CreateTextureFromSurface(ren, surf_val_nb);
                SDL_FreeSurface(surf_val_nb);
                int wv, hv;
                SDL_QueryTexture(texte_val_nb, NULL, NULL, &wv, &hv);
                SDL_Rect dst_val_nb = {case_rect.x + (case_rect.w - wv) / 2,
                                        case_rect.y + (case_rect.h - hv) / 2, wv, hv};
                SDL_RenderCopy(ren, texte_val_nb, NULL, &dst_val_nb);
                SDL_DestroyTexture(texte_val_nb);

                index_case++;
            }
        }


        // Paramètres de bouton Sauvegarder
        SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
        SDL_RenderFillRect(ren, &bouton_sauv);
        SDL_SetRenderDrawColor(ren, 140, 125, 90, 255);
        SDL_RenderDrawRect(ren, &bouton_sauv);
        SDL_Rect dst_sauv = {bouton_sauv.x + (bouton_sauv.w - wbs) / 2,
                            bouton_sauv.y + (bouton_sauv.h - hbs) / 2, wbs, hbs};
        SDL_RenderCopy(ren, texte_btn_sauv, NULL, &dst_sauv);


        //Paramètres de bouton Revenir
        SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
        SDL_RenderFillRect(ren, &bouton_rev);
        // contour
        SDL_SetRenderDrawColor(ren, 140, 125, 90, 255);
        SDL_RenderDrawRect(ren, &bouton_rev);
        SDL_Rect dst_btn_rev = {bouton_rev.x + (bouton_rev.w - wbr) / 2,
                                bouton_rev.y + (bouton_rev.h - hbr) / 2, wbr, hbr};
        SDL_RenderCopy(ren, texte_btn_rev, NULL, &dst_btn_rev);

    }

    else if (etat == ETAT_SIMULATION) {
        
        // fond noir pour la simulation
        SDL_Rect zone_monde = {0, 0, 1000, 800};
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderFillRect(ren, &zone_monde);

        // fond vert pour le panneau d'information
        SDL_Rect zone_panneau = {1000, 0, 400, 800};
        SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
        SDL_RenderFillRect(ren, &zone_panneau);

        // ligne de séparation entre les deux zones
        SDL_SetRenderDrawColor(ren, 245, 245, 220, 255);
        SDL_RenderDrawLine(ren, 1000, 0, 1000, 800);

        // bouton Start
        SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
        SDL_RenderFillRect(ren, &bouton_sim_start);
        SDL_SetRenderDrawColor(ren, 140, 125, 90, 255);
        SDL_RenderDrawRect(ren, &bouton_sim_start);
        SDL_Rect dst_sim_start = {bouton_sim_start.x + (bouton_sim_start.w - wss) / 2,
                                  bouton_sim_start.y + (bouton_sim_start.h - hss) / 2, wss, hss};
        SDL_RenderCopy(ren, texte_sim_start, NULL, &dst_sim_start);


        // bouton Pause
        SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
        SDL_RenderFillRect(ren, &bouton_sim_pause);
        SDL_SetRenderDrawColor(ren, 140, 125, 90, 255);
        SDL_RenderDrawRect(ren, &bouton_sim_pause);
        SDL_Rect dst_sim_pause = {bouton_sim_pause.x + (bouton_sim_pause.w - wsp) / 2,
                                bouton_sim_pause.y + (bouton_sim_pause.h - hsp) / 2, wsp, hsp};
        SDL_RenderCopy(ren, texte_sim_pause, NULL, &dst_sim_pause);


        // bouton Etape
        SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
        SDL_RenderFillRect(ren, &bouton_sim_etape);
        SDL_SetRenderDrawColor(ren, 140, 125, 90, 255);
        SDL_RenderDrawRect(ren, &bouton_sim_etape);
        SDL_Rect dst_sim_etape = {bouton_sim_etape.x + (bouton_sim_etape.w - wse) / 2,
                                bouton_sim_etape.y + (bouton_sim_etape.h - hse) / 2, wse, hse};
        SDL_RenderCopy(ren, texte_sim_etape, NULL, &dst_sim_etape);


        // bouton Acceleration
        SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
        SDL_RenderFillRect(ren, &bouton_sim_accel);
        SDL_SetRenderDrawColor(ren, 140, 125, 90, 255);
        SDL_RenderDrawRect(ren, &bouton_sim_accel);
        SDL_Rect dst_sim_accel = {bouton_sim_accel.x + (bouton_sim_accel.w - wsa) / 2,
                                bouton_sim_accel.y + (bouton_sim_accel.h - hsa) / 2, wsa, hsa};
        SDL_RenderCopy(ren, texte_sim_accel, NULL, &dst_sim_accel);


        // mouvement + faim (chaque tour)
        if (simulation_active == 1) {
            Uint32 maintenant = SDL_GetTicks();
            if (maintenant - dernier_tour >= (Uint32)delai_tour) {
                for (int i = 0; i < nb_individus; i++) {
                    if (individus[i]->vivant == 1)
                        deplacer_individu(individus[i]);
                        mourir_de_faim(individus[i]);
                        mourir_de_vieillesse(individus[i]);
                        individus[i]->tours_depuis_repro++;
                }
                dernier_tour = maintenant;
            }
        }

        // dessiner tous les individus
        for (int i = 0; i < nb_individus; i++) {
            if (individus[i]->vivant == 1){

                // couleur selon l'espece
                if (individus[i]->espece == &especes_predateurs[0])
                    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);

                else if (individus[i]->espece == &especes_predateurs[1])
                    SDL_SetRenderDrawColor(ren, 180, 0, 0, 255);

                else if (individus[i]->espece == &especes_herbivores[0])
                    SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);

                else if (individus[i]->espece == &especes_herbivores[1])
                    SDL_SetRenderDrawColor(ren, 0, 180, 0, 255);

                else if (individus[i]->espece == &especes_oiseaux[0])
                    SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);

                else if (individus[i]->espece == &especes_oiseaux[1])
                    SDL_SetRenderDrawColor(ren, 200, 200, 0, 255);

                else if (individus[i]->espece == &especes_poissons[0])
                    SDL_SetRenderDrawColor(ren, 0, 150, 255, 255);

                else if (individus[i]->espece == &especes_poissons[1])
                    SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);

                else if (individus[i]->espece == &especes_herbes[0])
                    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

                SDL_Rect rect = {individus[i]->x, individus[i]->y, individus[i]->taille, individus[i]->taille};
                SDL_RenderFillRect(ren, &rect);
            }
        }
    }

    // Affichage du frame
    SDL_RenderPresent(ren);
}

/* 4. Nettoyage */
TTF_CloseFont(font);
TTF_Quit();
SDL_DestroyTexture(fond_menu);
SDL_DestroyTexture(fond_parametres);
SDL_DestroyTexture(texte_btn);
SDL_DestroyTexture(texte_btn_par);
SDL_DestroyTexture(texte_btn_rev);
SDL_DestroyTexture(texte_btn_sauv);
SDL_DestroyTexture(texte_sim_start);
SDL_DestroyTexture(texte_sim_pause);
SDL_DestroyTexture(texte_sim_etape);
SDL_DestroyTexture(texte_sim_accel);
IMG_Quit();
SDL_DestroyRenderer(ren);
SDL_DestroyWindow(win);
SDL_Quit();

return 0;
}