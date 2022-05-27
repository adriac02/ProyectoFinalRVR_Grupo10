//Prueba
// std 
#include <stdio.h>

// opengl
#include <GL/glew.h>

// sdl
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600

int main (int argc, char* argv[])
{
    // ----- Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL could not initialize\n");
        return 1;
    }

    // ----- Create window
    SDL_Window* window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Error creating window.\n");
        return 2;
    }

    // ----- SDL OpenGL settings
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // ----- SDL OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // ----- SDL v-sync
    SDL_GL_SetSwapInterval(1);

    // ----- GLEW
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ----- Game loop
    bool quit = false;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    while (quit == false)
    {
        SDL_Event windowEvent;

        while (SDL_PollEvent(&windowEvent))
        {
            SDL_Texture* t = nullptr;
            SDL_Rect r; 
            r.x = 20;
            r.y = 20;
            r.w = 50;
            r.h = 50;
            SDL_Surface* surface = IMG_Load("../Assets/calvoLoco.jpeg");
            SDL_RendererFlip flip;

            SDL_Rect src; 
            src.x = 0;
            src.y = 0;
            src.w = surface->w;
            src.h = surface->h;


            t = SDL_CreateTextureFromSurface(renderer, surface);


            SDL_RenderCopyEx(renderer, t, &src, &r, 0, 0, flip);

            if (windowEvent.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
        }

        /*
            do drawing here
        */

        SDL_GL_SwapWindow(window);
    }

    // ----- Clean up
    SDL_GL_DeleteContext(glContext);

    return 0;
}