#ifndef APP_HPP
#define APP_HPP

#include "SDL3/SDL.h"

#include "Renderer.hpp"
#include "RenderState.hpp"
#include "SceneManager.hpp"

class App {
public:
    App(int argc, char* argv[]) : _argc(argc), _argv(argv), _renderer(_argc, _argv) {};

    void run();

private:
    int _argc;
    char** _argv;

    Renderer _renderer;
    SceneManager _sceneManager;

    SDL_Window* _window = nullptr;
    bool shouldClose = false;

    void init();
    void mainLoop();
    void cleanup();
};

#endif