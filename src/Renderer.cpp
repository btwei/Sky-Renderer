#include "Renderer.hpp"

#include "SDL3/SDL_vulkan.h"
#include "VkBootstrap.h"

void Renderer::init(SDL_Window* window) {
    _window = window;

    initVulkan();
}

void Renderer::drawFrame(const RenderState& state) {

}

void Renderer::cleanup() {

    SDL_Vulkan_DestroySurface(_instance, _surface, nullptr);

    // Fun comment -- vkb handles fetching this function, wow! No Volk or vkGetInstanceProcAddr needed!
    vkb::destroy_debug_utils_messenger(_instance, _debugMessenger);
    vkDestroyInstance(_instance, nullptr);
}

void Renderer::initVulkan() {
    // Create instance
    vkb::InstanceBuilder instanceBuilder;
    auto inst_ret = instanceBuilder.set_app_name("Sky Rendering Demo")
    #ifndef NDEBUG
        .request_validation_layers()
    #endif
        .use_default_debug_messenger()
        .build();
    
    if(!inst_ret) throw std::runtime_error("Failed to create Vulkan instance!");
    vkb::Instance vkb_inst = inst_ret.value();
    _instance = vkb_inst.instance;
    _debugMessenger = vkb_inst.debug_messenger;

    // Create surface via SDL (handles platform specific creation)
    SDL_Vulkan_CreateSurface(_window, _instance, nullptr, &_surface);

    // Pick physicalDevice based on compatibility with surface and extension support

}