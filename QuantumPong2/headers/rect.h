#pragma once

#include "window.h"
#include "paddle.h"
#include "barrier.h"
#include <string>

class Rect{
public:
    Rect(int w, int h, int x, int y , int r ,int g, int b, int a);
    Rect( int w, int h, int x, int y , int vx, int vy, std::string &image_path);
    Rect(int w, int h, int x, int y, int vx, int vy);
    ~Rect();
    void draw() const;
    void update(Window &window, Paddle &paddle, Barrier &barrier );
    void pollEvents(SDL_Event &event, Rect &rect);
    int get_vy(){ return this-> _vy;}
    int get_vx(){ return this-> _vx;}
    int get_x(){ return this-> _x;}
    int get_y(){ return this-> _y;}
    bool get_state(){return this-> _isQuantum;}
    void set_state(bool state){this->_isQuantum = state;}
    bool get_change(){return this-> _stateChanged;}
    void set_image_path(std::string &image_path);
private:
    bool _isQuantum = false;
    bool _isCharged = true;
    bool _stateChanged;
    int _charge = 0; 
    int _w, _h;
    int _x, _y;
    int _vx, _vy;
    int _r, _g, _b, _a;
    int _hits;
    SDL_Texture* _tennis_texture = nullptr;
    std::string _image_path;
    std::string _rootpath = "/Users/uqmreev4/Dropbox/QuantumPong2/QuantumPong2/res/";
    std::string _c_path =  _rootpath + "classical_ball.png";
    std::string _q_path = _rootpath + "cat_ball.png";
    std::string _pos_path  = _rootpath + "positive_ball.png";
    std::string _neg_path  = _rootpath + "negative_ball.png";
};
