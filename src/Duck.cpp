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