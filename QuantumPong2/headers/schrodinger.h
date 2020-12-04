#pragma once

#include <math.h>
#include "fftw3.h"
#include <complex>
#include <SDL2/SDL.h>
#include <string>
#include "barrier.h"
#include <vector>

class Schrodinger {
public:
    Schrodinger(int Mx, int My, Barrier &barrier);
    ~Schrodinger();
    void evolve(float DT, float dt);
    void initialize(float x, float y, float sigma , float kx, float ky);
    void draw();
    void save();
    Uint32* pixels;
    
private:
    int _Mx, _My;
    int _savenum=0;
    fftwf_plan _planf, _planb;
    float* _K2;
    float* _V;
    float *_x, *_y, *_kx, *_ky;
    std::complex<float> (*_psi0);
    std::vector<std::complex<float>> _psi;
    //std::complex<float> *_psi ;
    fftwf_complex* _psip;
    void fftshift(int Mx, int My, float* array);
    SDL_Texture * _texture;
    std::complex<float> I;
    
};



