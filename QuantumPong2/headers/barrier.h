#pragma once

#include "window.h"
#include "paddle.h"
#include <string>

class Barrier{
public:
    Barrier( int w, int h, int x, int y, const std::string &image_path);
    Barrier(int w, int hole_h, int n_holes ,int r, int g, int b, int a);
    ~Barrier();
    void draw(int wh,int ww) const;
    void update(Window &window, Paddle &paddle );
    void pollEvents(SDL_Event &event, Barrier &barrier);
    int get_w(){ return this-> _w;}
    int get_h(){ return this-> _h;}
    int get_n(){return this-> _n_holes;}
private:
    bool _isOn;
    int _w,_h;
    int _n_holes;
    int _r, _g, _b, _a;
    SDL_Texture* _barrier_texture = nullptr;
    const std::string _image_path;
};

