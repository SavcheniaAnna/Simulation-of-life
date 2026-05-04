#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

// Taille de la fenetre
#define LARGEUR 1400
#define HAUTEUR 800

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
if (!fond_menu) {
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

// Les lignes de texte a afficher
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
    "combien d'individus de chaque espèce vous voulez. Attention !!  Il faut",
    "minimum 2 individus de la même espèce pour qu'ils puissent se reproduire !"
};
int nb_lignes = 10;


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
    }

//------------------------------------------------------------------------------------

    // Fond noir
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

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


    // Affichage du frame
    SDL_RenderPresent(ren);
}

    /* 4. Nettoyage */
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(fond_menu);
    IMG_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return 0;
}