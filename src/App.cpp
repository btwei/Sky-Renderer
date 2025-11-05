#include "App.hpp"

#include <stdexcept>

void App::run() {
    init();
    mainLoop();
    cleanup();
}

void App::init() {
    // Initialize SDL3 window
    SDL_Init(SDL_INIT_VIDEO);

    _window = SDL_CreateWindow(
        "Sky Rendering Demo",
        640, 480,
        SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_VULKAN
    );

    if(_window == nullptr)
        throw std::runtime_error("Failed to create SDL3 window!");

    // Initialize renderer
    _renderer.init(_window);

    // Initialize scene manager
    _sceneManager.init();
}

void App::mainLoop() {
    // Handle updating and rendering on the main thread
    while(!shouldClose) {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                shouldClose = true;
            }

            // Process inputs
            _sceneManager.handleInput(event);
        }

        RenderState sceneState = _sceneManager.writeSceneState();

        _renderer.drawFrame(sceneState);
    }
}

void App::cleanup() {
    _sceneManager.cleanup();

    _renderer.cleanup();

    SDL_DestroyWindow(_window);

    SDL_Quit();
}