#include "Duck.h"

Duck::Duck(SDL_Renderer* r, SDL_Texture* t) : GameObject(r,t)
{

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

bool Duck::checkShot(SDL_Point p){
    if(SDL_PointInRect(&p, &dest)){
        alive = false;
        return true;
    }
    return false;
}