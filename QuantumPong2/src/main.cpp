#include "window.h"
#include "rect.h"
#include "paddle.h"
#include "barrier.h"
#include <math.h>
#include <complex>
#include <cstdlib>
#include <iostream>
#include "text.h"
#include "fftw3.h"
#include "schrodinger.h"
#include <thread>
#include "mpi.h"

void pollEvents(Window &window, Rect &rect, Paddle &paddle){
    SDL_Event event;
    if (SDL_PollEvent(&event)){
        rect.pollEvents(event,rect);
        paddle.pollEvents(event,window);
        window.pollEvents(event);
        
        
    }
}

int main(int argc, const char * argv[]) {
    
    int WIDTH = 800;
    int HEIGHT = 600;
    Uint32 FPS = 100;
    Window window("Quantum Pong",WIDTH,HEIGHT);
    //std::string rootpath = "/Users/uqmreev4/Dropbox/QuantumPong2/QuantumPong2/res/";
    std::string rootpath = "/Users/mattreeves/Dropbox/QuantumPong2/QuantumPong2/res/";
    std::string c_path =  rootpath + "classical_ball.png";
    std::string q_path = rootpath + "quantum_ball.png";
    const std::string t_path = rootpath + "Hack-Bold.ttf";
     std::string p_path = rootpath + "Paddle.png";
    
    Rect rect(30,30,300,300,0,0,c_path);
    Text text(Window::renderer,t_path,20, "Quantum Pong", {255,255,255,255});
    Paddle paddle(30,100,0,0,p_path);
    Barrier barrier(10, 90, 2,100, 100, 100, 255);
    Schrodinger schrodinger(WIDTH/2,HEIGHT/2,barrier);
    schrodinger.initialize(rect.get_x()/2-20, rect.get_y()/2,10, 100,50);
    //for some reason, this splits in two when ky =0 now ... MTR 12/01/20
    
    while (!window.isClosed()){
        Uint32 start_time = SDL_GetTicks();
        
        //std::thread t1(&Schrodinger::evolve,&schrodinger,2,0.75);
        schrodinger.evolve(5,5);

        pollEvents(window,rect,paddle);
        rect.update(window,paddle,barrier);
        schrodinger.draw();
        paddle.draw();
        rect.draw();
        barrier.draw(window.get_h(),window.get_w());
        text.display(30,550,Window::renderer);
        window.clear();
        
        Uint32 end_time = SDL_GetTicks();
        Uint32 delayMs = 1000/FPS - (end_time - start_time); //how long to delay
        
        //Maintain the framerate.
        if(1000/FPS>(end_time-start_time))
        {
            SDL_Delay(delayMs);
            std::cout << "delay is " <<  delayMs << std::endl;
        }
        
    }
    
    return 0;
}
    
