#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

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

    // creates a surface to load an image into the main memory
    SDL_Surface* surface;

    // please provide a path for your image
    surface = IMG_Load("Assets/pato.png");
    

    // loads image to our graphics hardware memory.
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_RenderCopy(rend, tex, NULL, NULL);
    SDL_RenderPresent(rend);
    // clears main-memory
    SDL_FreeSurface(surface);

    
    // let us control our image position
    // so that we can move it with our keyboard.
    SDL_Rect dest;

    // connects our texture with dest to control position
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

    dest.w /= 6;
    dest.h /= 6;
    // sets initial x-position of object
    dest.x = (1000 - dest.w) / 2;

    // sets initial y-position of object
    dest.y = (1000 - dest.h) / 2;


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

        

        SDL_SetRenderDrawColor( rend, 0, 255, 0, 255 );
        //SDL_RenderClear(rend);

        SDL_RenderCopy(rend, tex, NULL, &dest);

        // SDL_Rect r;
        // r.x = 50;
        // r.y = 50;
        // r.w = 150;
        // r.h = 150;

        // // Set render color to blue ( rect will be rendered in this color )
        // SDL_SetRenderDrawColor( rend, 0, 0, 255, 255 );

        // // Render rect
        // SDL_RenderFillRect( rend, &r );

        // clears the screen
        // SDL_RenderClear(rend);
        // SDL_RenderCopy(rend, tex, NULL, &dest);

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