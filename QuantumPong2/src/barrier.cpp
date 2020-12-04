#include "barrier.h"
#include "window.h"
#include "paddle.h"
#include <SDL2_image/SDL_image.h>
#include <iostream>

Barrier::Barrier(int w, int hole_h, int n_holes ,int r, int g, int b, int a):
_w(w), _h(hole_h), _n_holes(n_holes), _r(r), _g(g), _b(b), _a(a)
{
    //auto surface = IMG_Load(image_path.c_str());
    //std::cout << image_path.c_str() << std::endl;
    //if (!surface) std::cerr << "Failed to create surface\n.";
    //_barrier_texture = SDL_CreateTextureFromSurface(Window::renderer, surface);
    //if(!_barrier_texture){
    //    std::cerr << "Failed to create texture\n.";
    //}
    //SDL_FreeSurface(surface);
}
 
Barrier::~Barrier(){
    //SDL_DestroyTexture(_barrier_texture);
}

void Barrier::draw(int wh, int ww) const   {
    
    SDL_Rect barrier = {ww/2-_w/2,wh/2-_h/2,_w,_h};
    SDL_Rect barrier_u = {ww/2-_w/2,0,_w,wh/2-3*(_h/2)};
    SDL_Rect barrier_l = {ww/2-_w/2,wh/2+3*_h/2,_w,wh};
    
    if (_barrier_texture){
        SDL_RenderCopy(Window::renderer,_barrier_texture,nullptr,&barrier);
    }
    else{
        SDL_SetRenderDrawColor(Window::renderer,_r,_g,_b,_a);
        SDL_RenderFillRect(Window::renderer, &barrier);
        SDL_RenderFillRect(Window::renderer, &barrier_u);
        SDL_RenderFillRect(Window::renderer, &barrier_l);
    }
    
}

void Barrier::pollEvents(SDL_Event &event, Barrier &Barrier){}
    


//
