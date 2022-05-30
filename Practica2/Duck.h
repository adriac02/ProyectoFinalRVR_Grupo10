#include "GameObject.h";

class Duck : public GameObject
{
private:
    /* data */
float vel = 3;

public:
    bool alive = true;
    int puntos = 10;
    bool gold = false;

    Duck(SDL_Renderer* r, SDL_Texture* t, bool g);
    ~Duck();

    virtual void update();
    bool checkShot(SDL_Point p);

    void setVel();
};


