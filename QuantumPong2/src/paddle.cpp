#include "paddle.h"
#include "window.h"
#include <SDL2_image/SDL_image.h>
#include <iostream>

Paddle::Paddle(int w, int h, int x, int y , int r ,int g, int b, int a):
_w(w), _h(h), _x(x), _y(y), _r(r), _g(g), _b(b), _a(a)
{
}

Paddle::Paddle(int w, int h, int x, int y,  std::string &image_path ):
_w(w), _h(h), _x(x), _y(y)
{
    auto surface = IMG_Load(image_path.c_str());
    std::cout << image_path.c_str() << std::endl;
    if (!surface) std::cerr << "Failed to create surface\n.";
    _paddle_texture = SDL_CreateTextureFromSurface(Window::renderer, surface);
    if(!_paddle_texture){
        std::cerr << "Failed to create texture\n.";
    }
    SDL_FreeSurface(surface);
}

Paddle::~Paddle(){
    SDL_DestroyTexture(_paddle_texture);
}

void Paddle::draw() const   {
    SDL_Rect rect = {_x,_y,_w,_h};
    if (_paddle_texture){
        SDL_RenderCopy(Window::renderer,_paddle_texture,nullptr,&rect);
    }
    else{
        SDL_SetRenderDrawColor(Window::renderer,_r,_g,_b,_a);
        SDL_RenderFillRect(Window::renderer, &rect);
    }
}

void Paddle::update(){
    
}

void Paddle::set_image_path(std::string &image_path){
    auto surface = IMG_Load(image_path.c_str());
    std::cout << image_path.c_str() << std::endl;
    if (!surface) std::cerr << "Failed to create surface\n.";
    _paddle_texture = SDL_CreateTextureFromSurface(Window::renderer, surface);
    if(!_paddle_texture){
        std::cerr << "Failed to create texture\n.";
    }
    SDL_FreeSurface(surface);
}

void Paddle::pollEvents(SDL_Event &event, Window &window){
    switch (event.type){
    case SDL_MOUSEMOTION:
         //Probably not the best way to do this; maybe use SDL_GetWindowSize() ?.
            //Learn more about classes and check this again ... 
        //_y = event.motion.y < Window::get_h() - _h ? event.motion.y:Window::get_h() - _h;
            _y = event.motion.y < window.get_h() - _h ? event.motion.y : window.get_h() - _h;
        break;
    default:
        break;
    }
    
    
    std::string neg_path = "/Users/uqmreev4/Dropbox/QuantumPong2/QuantumPong2/res/negative_paddle.png";
    std::string pos_path = "/Users/uqmreev4/Dropbox/QuantumPong2/QuantumPong2/res/positive_paddle.png";
    std::string neu_path = "/Users/uqmreev4/Dropbox/QuantumPong2/QuantumPong2/res/Paddle.png";
    
    
    if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym){
            case SDLK_p:
                charge = +1;
                std::cout << charge << std::endl;
                set_image_path(pos_path);
                break;
            case SDLK_m:
                charge = -1;
                std::cout << charge << std::endl;
                set_image_path(neg_path);
                break;
            case SDLK_n:
                charge = 0;
                std::cout << charge << std::endl;
                set_image_path(neu_path);
                break;
            case SDLK_SPACE:
                charge *= -1;
                std::cout << charge << std::endl;
                if (charge == -1) set_image_path(neg_path);
                if (charge == +1) set_image_path(pos_path);
                break;
        }
    }
}
