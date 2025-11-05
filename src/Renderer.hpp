#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "SDL3/SDL.h"

#include "RenderState.hpp"

class Renderer {
public:
    Renderer(int argc, char** argv) : _argc(argc), _argv(argv) {};

    void init(SDL_Window* window);
    void drawFrame(const RenderState& state);
    void cleanup();
private:
    int _argc;
    char** _argv;

    SDL_Window* _window;

    
};

#endif