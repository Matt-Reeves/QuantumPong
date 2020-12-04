#pragma once

#include <string>
#include <SDL2/SDL.h>

class Window {
public: 
    Window(const std::string &title, int width, int height);
    ~Window();
    void pollEvents(SDL_Event &event);
    void clear() const;
    inline bool isClosed() const {return _closed;}
    int get_h(){ return _height;}
    int get_w(){ return _width;}
    
private:
    bool init();
    
private:
    std::string _title;
    int _width = 800;
    int _height= 600;
    
    bool _closed = false;
    SDL_Window* _window = nullptr;
    
public:
    static SDL_Renderer* renderer;
    
};


