#include "Duck.h"

Duck::Duck(SDL_Renderer* r, SDL_Texture* t, bool g) : GameObject(r,t)
{
    gold = g;
}

Duck::~Duck()
{

}

void Duck::update(){
    dest.x += vel;
}

void Duck::setVel(){
    vel = -vel;
}

void Duck::updateVel(float v){
    vel = v;
}

bool Duck::checkShot(SDL_Point p){
    if(SDL_PointInRect(&p, &dest)){
        alive = false;
        return true;
    }
    return false;
}