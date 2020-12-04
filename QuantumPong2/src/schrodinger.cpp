#include "schrodinger.h"
#include "fftw3.h"
#include <complex.h>
#include <cmath>
#include <iostream>
#include "barrier.h"
//#include <mpi.h>
#include <thread>

Schrodinger::Schrodinger(int Mx, int My, Barrier &barrier):
_Mx(Mx), _My(My) {
    
    I = std::complex<float> (0,1);
    _psi = std::vector<std::complex<float>> (My*Mx);
    pixels = new Uint32[Mx*My];
    _psip = reinterpret_cast< fftwf_complex*>(&(_psi[0]));
    
    std::cout << "Planning FFTs ..." ;
    fftw_plan_with_nthreads(4);
    _planf = fftwf_plan_dft_2d(My, Mx, _psip, _psip, FFTW_FORWARD, FFTW_PATIENT);
    _planb = fftwf_plan_dft_2d(My, Mx, _psip, _psip, FFTW_BACKWARD,FFTW_PATIENT);
    std::cout << "done" << std::endl;
    
    std::cout << "Initializing arrays ..." ;
    _K2 = new float[My*Mx];
    _V = new float[My*Mx];
    _x = new float[My*Mx];
    _y = new float[My*Mx];
    int i;
    for (i=0;i<_Mx*_My;i++){
        
        float kx, ky;
        kx = (float) (M_PI/_Mx)*(-_Mx + 2*(i%_Mx));
        ky = (float) (M_PI/_My)*(-_My + 2*(i/_Mx));
        _K2[i] = 0.5*sqrtf(kx*kx + ky*ky);
        _x[i] = i%_Mx;
        _y[i] = i/_Mx;
        _V[i] = (float) (  (abs(_x[i]-_Mx/2) < barrier.get_w())
                         & (abs(_y[i] - _My/2 ) < 5 ) );
                           //| abs(_y[i] - _My/2) > 20 ) );
        _V[i] *= 2.0f;
    }
    fftshift(_Mx, _My, &(_K2[0]));
    std::cout << "done" << std::endl;
    
    _texture = SDL_CreateTexture(Window::renderer,
                                              SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, Mx, My);
}

Schrodinger::~Schrodinger(){
    fftwf_destroy_plan(_planb);
    fftwf_destroy_plan(_planf);
    delete[] _V;
    delete[] _K2;
    delete[] _x;
    delete[] _y;
    delete[] pixels;
    SDL_DestroyTexture(_texture);
}

void Schrodinger::initialize(float x0, float y0, float sigma,float kx, float ky){
    int i;
    std::cout << "Initializing wavefunction ..";
    kx *= 2.0*M_PI/_Mx;
    ky *= 2.0*M_PI/_My;
    
    for (i=0; i<_My*_Mx;i++){
        _psi[i] = std::exp(-((_x[i]-x0)*(_x[i]-x0) +
                             (_y[i]-y0)*(_y[i]-y0))/sigma/sigma);
        _psi[i] *= std::exp(I*(kx*_x[i] + ky*_y[i]));
    }
    fftwf_execute(_planf);
    std::cout << "done" << std::endl;
}

void Schrodinger::evolve(float DT, float dt){
    int Nt = floor(DT/dt);
    int i,j;
    for (i=0;i<Nt;i++){
        for (j=0;j<_Mx*_My;j++)
            _psi[j] *= std::exp(-I*_K2[j]*dt/2.0f);
        fftwf_execute(_planb);
        for (j=0;j<_Mx*_My;j++)
            _psi[j] *=  std::exp(-I*_V[j]*dt)/((float) (_Mx*_My));
        fftwf_execute(_planf);
        for (j=0;j<_Mx*_My;j++)
            _psi[j] *= std::exp(-I*_K2[j]*dt/2.0f);
    }
}

void Schrodinger::fftshift(int Mx, int My,float* array){
    
    int N = Mx*My;
    int i;
    float temp;
    float* temp2 = (float*) malloc(N*sizeof(float));
    
    // interchange entries in 4 quadrants, 1 <--> 3 and 2 <--> 4
    if (N%2 !=0){
        std::cerr << "Error: Number of points must be even for fftshift to work correctly\n" ;
    }
    //Wrapping in y ...
    for (i=0;i<N/2;i++){
        temp = array[i];
        array[i] = array[i+N/2];
        array[i+N/2] = temp;
        
    }
    
    int m2 = Mx/2;
    //wapping in x ...
    for (i=0;i<N;i++){
        if (i%Mx < m2){
            // printf("i%%m2 = %d\n",i%m2);
            temp2[i+m2] = array[i];
        }
        else
            temp2[i-m2] = array[i];
    }
    
    for (i=0;i<N;i++) array[i] = temp2[i];
    
    free(temp2);
    
}

void Schrodinger::draw(){
    int i;
    static const int numthreads = 4;
    std::thread t[numthreads];
    std::complex<float> b;
    fftwf_execute(_planb);
    float maxpsi= 0.0;
    //for(j=0;j<numthreads;j++){
    //for (i=0+j*_Mx*_My/numthreads;i<_Mx*_My/numthreads;i++){
    
    for (i=0;i<_Mx*_My;i++){
        _psi[i] /= (_Mx*_My);
            b =  std::norm(_psi[i]);
            if (b.real() > maxpsi) maxpsi = b.real();
    }
    
    //}
    //std::cout << maxpsi << std::endl;
    //for(j=0;j<numthreads;j++){
    for (i=0;i<_Mx*_My;i++){
        pixels[i] = (Uint32) 255*((std::norm(_psi[i])/maxpsi));
        if (pixels[i] > 10){
            SDL_SetRenderDrawColor(Window::renderer, pixels[i], pixels[i]/2, pixels[i], 255);
            //SDL_RenderDrawPoint(Window::renderer, (i)%(_Mx), (i)/(_Mx));
            SDL_RenderDrawPoint(Window::renderer, (2*i)%(2*_Mx), (2*i)/(_Mx));
            SDL_RenderDrawPoint(Window::renderer, (2*i)%(2*_Mx)+1, (2*i)/(_Mx));
            SDL_RenderDrawPoint(Window::renderer, (2*i)%(2*_Mx), (2*i)/(_Mx)+1);
            SDL_RenderDrawPoint(Window::renderer, (2*i)%(2*_Mx)+1, (2*i)/(_Mx)+1);
        }
    }
//    }
//    for (j=0;j<numthreads;j++) t[j].join();
    //SDL_UpdateTexture(_texture, NULL, pixels, _Mx * sizeof(Uint32));
    //SDL_RenderCopy(Window::renderer, _texture, NULL, NULL);
    fftwf_execute(_planf);
}

//Save to .txt file  (for debugging purposes only)
void Schrodinger::save(){
    if (_savenum < 10){
        FILE *pfile1,*pfile2,*pfile3,*pfile4,*pfile5;
        char savefile_r [256];
        char savefile_i [256];
        char savefile_V [256];
        char savefile_K [256];
        char savefile_p [256];
        
        sprintf(savefile_r, "/Users/mattreeves/Desktop/%dreal.txt",_savenum);
        sprintf(savefile_i, "/Users/mattreeves/Desktop/%dimag.txt",_savenum);
        sprintf(savefile_K, "/Users/mattreeves/Desktop/%dK.txt",_savenum);
        sprintf(savefile_V, "/Users/mattreeves/Desktop/%dV.txt",_savenum);
        sprintf(savefile_p, "/Users/mattreeves/Desktop/%dpixels.txt",_savenum);
        pfile1 = fopen(savefile_r, "w");
        pfile2 = fopen(savefile_i, "w");
        pfile3 = fopen(savefile_K, "w");
        pfile4 = fopen(savefile_V, "w");
        pfile5 = fopen(savefile_p, "w");
        
        int i;
        for (i=0;i<_Mx*_My;i++){
            fprintf(pfile1,"%1.6f\n",_psi[i].real());
            fprintf(pfile2,"%1.6f\n",_psi[i].imag());
            fprintf(pfile3,"%1.6f\n",_K2[i]);
            fprintf(pfile4,"%1.6f\n",_V[i]);
            fprintf(pfile5,"%d\n",pixels[i]);
        }
        _savenum ++;
        fclose(pfile1);
        fclose(pfile2);
        fclose(pfile3);
        fclose(pfile4);
        fclose(pfile5);
    }
}

