#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "SDL3/SDL.h"

#include "RenderState.hpp"

class SceneManager {
public:
    SceneManager();

    void init();
    void handleInput(const SDL_Event& event);
    void stepScene(double dt);
    RenderState writeSceneState();
    void cleanup();
private:

};

#endif