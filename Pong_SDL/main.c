#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 780

//Option de build dans les linker setting mettre les libSDL2main.a libSDL2.dll.a
//Dans search directory
//Option de build compiler mettre les include
//Option de build linker mettre les lib

//mettre dans build linker -lmingw32

typedef struct game{

     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_ptexture;
     SDL_Surface *g_psurface;

}game;

typedef struct gameState{

    int g_bRunning;

}gameState;

typedef struct player{

    float speed;
    float x;
    float y;

}player;

typedef struct ball{

    float x;
    float y;
    float vx;
    float vy;
    float speed;

} ball;

//prototypes
int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame);
void destroy(game *myGame);
void handleEvents(gameState *state,player *joueur1,player *joueur2);
void delay(unsigned int frameLimit);
void renderTexture(game *myGame,player *joueur1,player *joueur2, ball *b);


int main(int argc, char *argv[]){

    game myGame;
    gameState state;

    unsigned int frameLimit = SDL_GetTicks() + 16;

    player joueur1;
    player joueur2;

    state.g_bRunning=1;

    joueur1.y  = (SCREEN_HEIGHT/2)-50;
    joueur2.y = (SCREEN_HEIGHT/2)-50;

    ball b;
    b.x = (SCREEN_WIDTH  / 2);
    b.y = (SCREEN_HEIGHT / 2);
    b.speed = 1;
    b.vx = (rand() % 2 == 0)? 1 : -1 * 1;
    b.vy = -2;

    //Create texture for drawing in texture or load picture
    myGame.g_ptexture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);

    init("Pong SDL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame);

    while(state.g_bRunning){
            handleEvents(&state,&joueur1,&joueur2);
            renderTexture(&myGame,&joueur1,&joueur2,&b);
    }

    system ("cls");

    // Gestion des 60 fps (1000ms/60 = 16.6 -> 16
    delay(frameLimit);
    frameLimit = SDL_GetTicks() + 16;

    destroy(&myGame);

    SDL_Quit();

    return 0;

}


int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame){

    myGame->g_pWindow=NULL;
    myGame->g_pRenderer=NULL;
    myGame->g_psurface=NULL;
    myGame->g_ptexture=NULL;

    //initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {
        //if succeeded create our window
        myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
        //if succeeded create window, create our render
        if(myGame->g_pWindow!=NULL){

            myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);

        }

    }

    else{

        return 0;

    }

    return 1;

}


void renderTexture(game *myGame,player *joueur1,player *joueur2, ball *b) {

    //Définition du rectangle 1 a dessiner
    SDL_Rect rectangle;
    rectangle.x=10;//debut x

    if(joueur1->y == 0 && joueur1->speed == -5 || (joueur1->y + 100) == SCREEN_HEIGHT && joueur1->speed == 5){

        joueur1->speed = 0;
    }

    joueur1->y =(joueur1->y) + (joueur1->speed);
    rectangle.y= joueur1->y;//debut y
    rectangle.w=10; //Largeur
    rectangle.h=100;//Hauteur

    //Définition du rectangle 2 a dessiner
    SDL_Rect rectangle2;
    rectangle2.x=SCREEN_WIDTH-20;//debut x

    if(joueur2->y == 0 && joueur2->speed == -5 || (joueur2->y + 100) == SCREEN_HEIGHT && joueur2->speed == 5){

        joueur2->speed = 0;
    }

    joueur2->y  =(joueur2->y) + (joueur2->speed);
    rectangle2.y=joueur2->y;//debut y
    rectangle2.w=10; //Largeur
    rectangle2.h=100;//Hauteur

    SDL_Rect balle;
    balle.w = 10;
    balle.h = 10;

    if(20 == b->x){

        if(joueur1->y <= b->y && (joueur1->y + 100) >= b->y){

            b->vx = b->vx * -1;
        }
    }

    else if ((SCREEN_WIDTH-30) == b->x){

        if(joueur2->y <= b->y && (joueur2->y + 100) >= b->y){

            b->vx = b->vx * -1;
        }
    }



    if(b->x == 0 || b->x == (SCREEN_WIDTH-10)){

        b->x = (SCREEN_WIDTH  / 2);
        b->y = (SCREEN_HEIGHT / 2);
    }

    else{

        b->x = (b->x)+(b->vx);
    }


    balle.x = b->x;


    if(b->y == 0 || b->y == (SCREEN_HEIGHT-10)){

        b->vy = b->vy*-1;
        b->y = (b->y)+(b->vy);

    }

    else{
        b->y = (b->y)+(b->vy);
    }

    balle.y = b->y;

    //Draw in texture
    SDL_SetRenderDrawColor(myGame->g_pRenderer,255,0,0,255);

    //on modifie la texture
    SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture);
    SDL_RenderFillRect(myGame->g_pRenderer, &rectangle);
    SDL_RenderFillRect(myGame->g_pRenderer, &rectangle2);
    SDL_RenderFillRect(myGame->g_pRenderer, &balle);
    // Dorénavent, on modifie à nouveau le renderer
    SDL_SetRenderTarget(myGame->g_pRenderer, NULL);

    //position de la texture dans screen
    SDL_Rect position;
    position.x = 0;
    position.y = 0;
    SDL_QueryTexture(myGame->g_ptexture, NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&position);

    SDL_RenderPresent(myGame->g_pRenderer);

    SDL_SetRenderDrawColor(myGame->g_pRenderer,0,0,0,255);
    SDL_DestroyTexture(myGame->g_ptexture);
    SDL_RenderClear(myGame->g_pRenderer);

}

void destroy(game *myGame){

    //Destroy render
    if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);

    //Destroy texture
    if(myGame->g_ptexture!=NULL)
        SDL_DestroyTexture(myGame->g_ptexture);

    //Destroy surface
    if(myGame->g_psurface!=NULL)
        SDL_FreeSurface(myGame->g_psurface);

    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}


void handleEvents(gameState *state,player *joueur1,player *joueur2){

    SDL_Event event;

    if(SDL_PollEvent(&event)){

        switch(event.type){

            case SDL_QUIT: state->g_bRunning=0; break;
            case SDL_KEYDOWN: switch (event.key.keysym.sym){

                case SDLK_z:     joueur2->speed = -5; break;
                case SDLK_s:     joueur2->speed = 5 ; break;
                case SDLK_UP:    joueur1->speed = -5; break;
                case SDLK_DOWN:  joueur1->speed = 5 ; break;

            } break;

            case SDL_KEYUP: switch (event.key.keysym.sym){

                case SDLK_z:     joueur2->speed = 0; break;
                case SDLK_s:     joueur2->speed = 0; break;
                case SDLK_UP:    joueur1->speed = 0; break;
                case SDLK_DOWN:  joueur1->speed = 0; break;

            } break;

            default:break;

        }

    }

}

void delay(unsigned int frameLimit){

    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks){

        return;

    }

    if (frameLimit > ticks + 16){

        SDL_Delay(16);

    }

    else{

        SDL_Delay(frameLimit - ticks);

    }

}
