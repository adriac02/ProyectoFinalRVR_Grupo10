#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

class GameObject
{
private:
    //Necessary for rendering
    SDL_Renderer* render = nullptr;
    SDL_Texture* text = nullptr;
    SDL_Rect* dest;

public:
    GameObject(SDL_Renderer* r, SDL_Texture* t);
    ~GameObject();

    void setPos(int x, int y);
    void setSize(int w, int h);
};