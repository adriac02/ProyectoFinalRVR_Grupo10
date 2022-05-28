#include "GameObject.h"

float duckSpawningTime = 5000;
float timeSinceLastSpawn = 0;

int main(int argc, char *argv[])
{
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
        printf("error initializing SDL: %s\n", SDL_GetError());

    SDL_Window* win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 1000, 0);

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;     
    int initted = IMG_Init(flags);     
    if ((initted & flags) != flags)         
    printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);


    SDL_Surface* surface;
    surface = IMG_Load("Assets/pato.png");
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_RenderCopy(rend, tex, NULL, NULL);
    SDL_RenderPresent(rend);

    // clears main-memory
    SDL_FreeSurface(surface);

    SDL_Rect dest;
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w /= 6;
    dest.h /= 6;
    dest.x = (1000 - dest.w) / 2;
    dest.y = (1000 - dest.h) / 2;

    SDL_Surface* pastoSurf;
    pastoSurf = IMG_Load("Assets/pasto.png");
    SDL_Texture* pastoTex = SDL_CreateTextureFromSurface(rend, pastoSurf);
    SDL_RenderCopy(rend, pastoTex, NULL, NULL);
    SDL_RenderPresent(rend);

    // clears main-memory
    SDL_FreeSurface(pastoSurf);
    
    SDL_Rect pastoDest;
    SDL_QueryTexture(pastoTex, NULL, NULL, &pastoDest.w, &pastoDest.h);
    pastoDest.x = (1000 - pastoDest.w) / 2;
    pastoDest.y = ((1000 - pastoDest.h) / 2) + 50;


    bool close = 0;

    // animation loop
    while (!close) {
        SDL_Event event;

        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;
            default:
                break;
            }
        }

        if(timeSinceLastSpawn >= duckSpawningTime){
            //Spawn a duck


            timeSinceLastSpawn = 0;
        }
        else timeSinceLastSpawn = timeSinceLastSpawn + SDL_GetTicks();
        

        SDL_SetRenderDrawColor( rend, 0, 170, 255, 255 );
        SDL_RenderClear(rend);

        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderCopy(rend, pastoTex, NULL, &pastoDest);

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }

    // destroy texture
    SDL_DestroyTexture(tex);

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);

    // close SDL
    SDL_Quit();

    return 0;
}