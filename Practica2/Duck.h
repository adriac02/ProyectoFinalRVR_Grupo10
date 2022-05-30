#include "GameObject.h";

class Duck : public GameObject
{
private:
    /* data */


public:
    float vel = 3;
    bool alive = true;
    int puntos = 10;
    bool gold = false;

    Duck(SDL_Renderer* r, SDL_Texture* t, bool g);
    ~Duck();

    virtual void update();
    bool checkShot(SDL_Point p);

    void setVel();
    void updateVel(float v);
};


