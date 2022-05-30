#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

class GameObject
{
private:
    //Necessary for rendering
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* text = nullptr;

protected:
    SDL_Rect dest;
    
public:

    GameObject(SDL_Renderer* r, SDL_Texture* t);
    ~GameObject();

    void setPos(int x, int y);
    void setSize(int w, int h);

    int getX();
    int getY();


    virtual void render();
    virtual void update(){};
};