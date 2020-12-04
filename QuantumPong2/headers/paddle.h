#pragma once

#include "window.h"
#include <string>

class Paddle  {
public:
    Paddle( int w, int h, int x, int y , int r ,int g, int b, int a);
    Paddle(int w, int h, int x, int y,  std::string &image_path);
    ~Paddle();
    void draw() const;
    void update();
    void pollEvents(SDL_Event &event,Window &window);
    int get_h(){ return this-> _h;}
    int get_w(){ return this-> _w;}
    int get_x(){ return this-> _x;}
    int get_y(){ return this-> _y;}
    int get_charge(){return this -> charge;}
    void set_image_path(std::string &image_path);
private:
    int _w, _h;
    int _x, _y;
    int _vx, _vy;
    int _r, _g, _b, _a;
    bool is_charged; 
    int charge;
    SDL_Texture* _paddle_texture = nullptr;
};


