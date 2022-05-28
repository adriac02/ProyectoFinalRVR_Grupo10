#include "GameObject.h";

class Duck : public GameObject
{
private:
    /* data */
float vel = 3;

public:
    Duck(SDL_Renderer* r, SDL_Texture* t);
    ~Duck();

    virtual void update();

    void setVel();
};


