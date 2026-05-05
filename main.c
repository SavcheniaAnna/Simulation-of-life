#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

// Taille de la fenetre
#define LARGEUR 1400
#define HAUTEUR 800

//Pour ouvrir différentes fenêtres en appuiant sur les boutons
#define ETAT_MENU        0
#define ETAT_PARAMETRES  1
#define ETAT_SIMULATION  2

int main(void)
{

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


//Bouton Revenir
SDL_Rect bouton_rev = {50, 30, 110, 35};
SDL_Surface *surf_btn_rev = TTF_RenderUTF8_Blended(font, "Revenir", blanc_casse);
SDL_Texture *texte_btn_rev = SDL_CreateTextureFromSurface(ren, surf_btn_rev);
SDL_FreeSurface(surf_btn_rev);
int wbr, hbr;
SDL_QueryTexture(texte_btn_rev, NULL, NULL, &wbr, &hbr);



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
                if (event.button.x >= bouton_rev.x &&
                    event.button.x <= bouton_rev.x + bouton_rev.w &&
                    event.button.y >= bouton_rev.y &&
                    event.button.y <= bouton_rev.y + bouton_rev.h)
                    etat = ETAT_MENU;
            }
        }
    }


    if (etat == ETAT_MENU) {

        //Afficher le fond_menu
        SDL_RenderCopy(ren, fond_menu, NULL, &dst);


        // Afficher chaque ligne de texte
        for (int i = 0; i < nb_lignes; i++) {
            if (lignes[i][0] == '\0') {  // ligne vide -> on saute
            continue;}
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

        //Afficher le texte
        SDL_Surface *surf = TTF_RenderUTF8_Blended(font, "Il faut minimum 2 individus de la même espèce pour qu'ils puissent se reproduire !", blanc_casse);
        SDL_Texture *texte = SDL_CreateTextureFromSurface(ren, surf);
        SDL_FreeSurface(surf);

        int w, h;
        SDL_QueryTexture(texte, NULL, NULL, &w, &h);
        SDL_Rect dst_texte = {420, 120, w, h};
        SDL_RenderCopy(ren, texte, NULL, &dst_texte);
        SDL_DestroyTexture(texte);


        //Paramètres de bouton Revenir
        SDL_SetRenderDrawColor(ren, 92, 78, 52, 255);
        SDL_RenderFillRect(ren, &bouton_rev);
        // contour
        SDL_SetRenderDrawColor(ren, 140, 125, 90, 255);
        SDL_RenderDrawRect(ren, &bouton_rev);

        SDL_Rect dst_btn_rev = {bouton_rev.x + (bouton_rev.w - wbr) / 2, bouton_rev.y + (bouton_rev.h - hbr) / 2, wbr, hbr};
        SDL_RenderCopy(ren, texte_btn_rev, NULL, &dst_btn_rev);

    }

    else if (etat == ETAT_SIMULATION) {
        
        // nouveau fenêntre
    }


    // Affichage du frame
    SDL_RenderPresent(ren);
}

    /* 4. Nettoyage */
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(fond_menu);
    SDL_DestroyTexture(fond_parametres);
    IMG_Quit();
    SDL_DestroyTexture(texte_btn);
    SDL_DestroyTexture(texte_btn_par);
    SDL_DestroyTexture(texte_btn);      
    SDL_DestroyTexture(texte_btn_par);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return 0;
}