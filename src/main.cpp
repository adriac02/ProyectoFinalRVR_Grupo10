#include "Duck.h"
#include <vector>

float duckSpawningTime = 15;
float timeSinceLastSpawn = 0;

float winH = 1000;
float winW = 1000;

int main(int argc, char *argv[])
{
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
        printf("error initializing SDL: %s\n", SDL_GetError());

    SDL_Window* win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       winW, winH, 0);

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
    dest.w /= 10;
    dest.h /= 10;
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

    std::vector<Duck*> ducks;

    SDL_Point click;

    int points = 0;

    
    // animation loop
    while (!close) {
        SDL_RenderClear(rend);
        SDL_Event event;

        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;

                case SDL_MOUSEBUTTONDOWN:

                if(event.button.button == SDL_BUTTON_LEFT){

                    click.x = event.motion.x;
                    click.y = event.motion.y;

                    for (auto d : ducks)
                    {
                        d->checkShot(click);
                    }
                    
                }
                break;
            default:
                break;
            }
        }

        if(timeSinceLastSpawn >= duckSpawningTime){
            //Spawn a duck
            Duck* d = new Duck(rend, tex);

            int side = rand() % 2;
            int x,y;

            if(side == 0){
              x = 0;      
            }
            else x = winW;
            y = rand() % (int)winH;

            d->setPos(x, y);
            d->setSize(dest.w, dest.h);

            if(side == 1) d->setVel();

            ducks.push_back(d);

            timeSinceLastSpawn = 0;
        }
        else timeSinceLastSpawn++;

        for(int i=0; i < ducks.size(); i++){
            if(!ducks[i]->alive){
                delete ducks[i];
                ducks.erase(ducks.begin() + i);
            }
        }
        
        for(auto d : ducks){
            d->update();
            d->render();
        }


        SDL_SetRenderDrawColor( rend, 0, 170, 255, 255 );
        SDL_RenderCopy(rend, pastoTex, NULL, &pastoDest);

        //SDL_RenderCopy(rend, tex, NULL, &dest);

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