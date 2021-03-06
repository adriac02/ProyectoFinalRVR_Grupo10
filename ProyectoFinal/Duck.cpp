#include "Duck.h"
#include <iostream>

Duck::Duck(SDL_Renderer* r, SDL_Texture* t, bool g, float winW) : GameObject(r,t)
{
    gold = g;
    winWidth = winW;
}

Duck::~Duck()
{

}

void Duck::update(){
    dest.x += vel;
    if(dest.x < -dest.w  - offScreenOffset || dest.w > winWidth + offScreenOffset){
        alive = false;
    } 
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