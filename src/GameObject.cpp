#include "GameObject.h"

GameObject::GameObject(SDL_Renderer* r, SDL_Texture* t){
    renderer = r;
    text = t;
}

GameObject::~GameObject(){

}

void GameObject::setPos(int x, int y){
    dest.x = x;
    dest.y = y;
}

void GameObject::setSize(int w, int h){
    dest.w = w;
    dest.h = h;
}

void GameObject::render(){
    SDL_RenderCopy(renderer, text, NULL, &dest);
}