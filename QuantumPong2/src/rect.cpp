#include "rect.h"
#include "window.h"
#include "paddle.h"
#include "barrier.h"
#include <stdlib.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>

Rect::Rect(int w, int h, int x, int y , int r ,int g, int b, int a):
_w(w), _h(h), _x(x), _y(y), _r(r), _g(g), _b(b), _a(a)
{

}

Rect::Rect(int w, int h, int x, int y ,int vx, int vy, std::string &image_path):
_w(w), _h(h), _x(x), _y(y), _vx(vx), _vy(vy)
{
    auto surface = IMG_Load(image_path.c_str());
    std::cout << image_path.c_str() << std::endl;
    if (!surface) std::cerr << "Failed to create surface\n.";
    _tennis_texture = SDL_CreateTextureFromSurface(Window::renderer, surface);
    if(!_tennis_texture){
        std::cerr << "Failed to create texture\n.";
    }
    SDL_FreeSurface(surface);
}

Rect::Rect(int w, int h, int x, int y, int vx, int vy): _w(w), _h(h), _x(x), _y(y), _vx(vx), _vy(vy)
{
}

Rect::~Rect(){
    SDL_DestroyTexture(_tennis_texture);
}

void Rect::draw() const   {
    SDL_Rect rect = {_x,_y,_w,_h};
    
    if (_tennis_texture){
        SDL_RenderCopy(Window::renderer,_tennis_texture,nullptr,&rect);
    }
    else{
        
    SDL_SetRenderDrawColor(Window::renderer,_r,_g,_b,_a);
    SDL_RenderFillRect(Window::renderer, &rect);
    }
    
}

void Rect::set_image_path(std::string &image_path){
    auto surface = IMG_Load(image_path.c_str());
    std::cout << image_path.c_str() << std::endl;
    if (!surface) std::cerr << "Failed to create surface\n.";
    _tennis_texture = SDL_CreateTextureFromSurface(Window::renderer, surface);
    if(!_tennis_texture){
        std::cerr << "Failed to create texture\n.";
    }
    SDL_FreeSurface(surface);
    _stateChanged = false;
}

void Rect::update(Window &window, Paddle &paddle, Barrier &barrier ){
    //Move the position by the velocities
    _x += _vx;
    _y += _vy;
    
    if (_x >= window.get_w()-_w) _vx *= -1; // Right rebound
    if (_y >= window.get_h()-_h) _vy *= -1; //Top rebound
    if (_y <= 0) _vy *= -1;                 //Bottom reound
    if (_x <= 0){                           //Reset if goes off left of screen
        _vx = 0; _vy =0;
        _x = (window.get_w() - _w)/2;
        _y = (window.get_h() - _h)/2;
    }
    
    if (_isCharged){
        std::cout << "Hello from charge section" << std::endl;
        int a = paddle.get_y();
        int b = a + paddle.get_h();
        double Ex, Ey;
        double x = (double) _x;
        double y = (double) _y;
        int lambda = 2000;
        Ex = -1/x*( (a-y)/sqrt(x*x + (a-y)*(a-y)) - (b-y)/sqrt(x*x + (b-y)*(b-y)) );
        Ex = Ex >  lambda/2 ? lambda : Ex;
        Ey = 1/sqrt(x*x + (b-y)*(b-y)) - 1/sqrt(x*x + (a-y)*(a-y));
        Ey = Ey >  lambda/2 ? lambda : Ey;
        std::cout << "Ex = " << Ex << std::endl;
        std::cout << "Ey = " << Ey << std::endl;
        _vx += (int) (lambda*_charge*paddle.get_charge()*Ex);
        _vy += (int) (lambda*_charge*paddle.get_charge()*Ey);
        if (_vx > 10) _vx = 10;
        if (_vx < -10) _vx = -10;
        if (_vy > 10) _vy = 10;
        if (_vy < -10) _vy = -10;
    }
    
    //Paddle Rebound
    if ( _x <= paddle.get_w() && _vx < 0
        && (
                (_y + _h > paddle.get_y() && _y < paddle.get_y() )
             || (_y + _h < paddle.get_y() + paddle.get_h() && _y > paddle.get_y() )
             || (_y+ _h  > paddle.get_y() + paddle.get_h() && _y < paddle.get_y() + paddle.get_h() )
            ) ){
            _vx *= -1;
            _hits++;
            std::cout << "hits = " << _hits << std::endl;
            if (_isCharged){
                _vx = 0;
                _vy = 0;
                //                if (_vx > 0) -- _vx;
                //                if (_vx < 0) ++ _vx;
                //                if (_vy > 0) -- _vy;
                //                if (_vy < 0) ++ _vy;
            }
        if (_hits%5 == 0){
//            if (_vx > 0)_vx++;
//            else _vx--;
//            if (_vy > 0)_vy++;
//            else _vy--;
//            if (_vx > 10) _vx = 10;
//            if (_vx < -10) _vx = -10;
//            if (_vy > 10) _vy = 10;
//            if (_vy < -10) _vy = -10;
        }
    }
    
    int xR = (barrier.get_w() + window.get_w())/2;
    int xL = (-barrier.get_w() + window.get_w())/2;
    int y1 = window.get_h()/2 - 3*barrier.get_h()/2;
    int y2 = window.get_h()/2 - barrier.get_h()/2;
    int y3 = window.get_h()/2 + barrier.get_h()/2 ;
    int y4 = window.get_h()/2 + 3*barrier.get_h()/2;
    
    //Conditions for barrier rebound
    bool x_cond1 = (_x <= xR & _x >= xL);
    bool x_cond2 = (_x+_w >= xL & _x+_w <= xR);
    bool x_cond3 = (_x+_w >= xR & _x <= xL);
    bool y_cond1 = _y+_h < y1;                  //x-rebound
    bool y_cond2 = _y+_h > y1 & _y <= y1;       //xy-rebound
    bool y_cond3 = _y < y2 & _y+_h >= y2;       //x-y-rebound
    bool y_cond4 = _y >= y2 & _y+_h <= y3;      //x-rebound
    bool y_cond5 = _y <= y3 & _y+_h > y3;       //x-y-rebound
    bool y_cond6 = _y < y4 & _y+_h >= y4;       //x-y-rebound
    bool y_cond7 = _y >= y4;                    //x-rebound
    
    //Logic for barrier rebound
    if ( (x_cond1 | x_cond2)  & (y_cond1  | y_cond4 | y_cond7 )){
        if (_isQuantum){
            float prob = ((float) rand() / (RAND_MAX));
            if (prob < 0.1) _vx *=-1;
        }
        else _vx *=-1;
    }
    if ( (x_cond1 | x_cond2)  & (y_cond2 | y_cond3  | y_cond5 | y_cond6  )){
        if (_isQuantum){
            float prob = ((float) rand() / (RAND_MAX));
            if (prob < 0.1){ _vx *=-1; _vy*=-1;}
        }
        else{ _vx *=-1; _vy*=-1;}
    }
     else if (x_cond3  & (y_cond2 | y_cond3  | y_cond5 | y_cond6  )){
         if (_isQuantum){
             float prob = ((float) rand() / (RAND_MAX));
             if (prob < 0.1) _vy *=-1;
         }
         else _vy *=-1;
    }
    

    
    
    if (_charge == 1)
        set_image_path(_pos_path);
    else if (_charge == -1)
        set_image_path(_neg_path);
    
    //Check for classical-quantum state change and change accordingly.
    else if (_stateChanged ){
        if (_isQuantum) set_image_path(_q_path);
        else set_image_path(_c_path);
        _charge = 0;
    }
    
 
    
    
}

void Rect::pollEvents(SDL_Event &event, Rect &rect){
    
    if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym){
               case SDLK_q:
                    _stateChanged = true;
                    _isQuantum = !_isQuantum;
                    std::cout << _isQuantum << std::endl;
                    break;
                case SDLK_LEFT:
                    _x -= 12;
                    break;
                case SDLK_RIGHT:
                    _x += 12;
                    break;
                case SDLK_UP:
                    _y -= 12;
                    break;
                case SDLK_DOWN:
                    _y += 12;
                    break;
                case SDLK_RETURN:
                    _vx += 4;
                    _vy += 4;
                    break;
                case SDLK_s:
                    _vx = 0;
                    _vy = 0;
                    break;
                case (SDLK_1):
                    _charge = +1;
                    break;
                case (SDLK_2):
                    _charge = -1;
                    break;
                case (SDLK_0):
                    _charge = 0;
                    break;
                    
            }

        }
}



//
