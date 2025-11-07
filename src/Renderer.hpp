#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "SDL3/SDL.h"
#include "vulkan/vulkan.h"

#include "RenderState.hpp"

static int NUM_FRAMES = 2;

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

    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debugMessenger;
    VkSurfaceKHR _surface;
    VkPhysicalDevice _physicalDevice;
    VkDevice _device;

    VkQueue _graphicsQueue;
    uint32_t _graphicsFamilyIndex;

    VkSwapchainKHR _swapchain;
    VkExtent2D _swapchainExtent;
    VkFormat _swapchainFormat;
    std::vector<VkImage> _swapchainImages;
    std::vector<VkImageView> _swapchainImageViews;

    VkPipeline _meshPipeline;

    VkPipeline _skyPipeline;

    int _frameNumber;
    
    void initVulkan();
};

#endif