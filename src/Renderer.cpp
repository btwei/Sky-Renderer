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

    vkDestroyDevice(_device, nullptr);

    SDL_Vulkan_DestroySurface(_instance, _surface, nullptr);

    // Fun comment -- vkb handles fetching this function, wow! No Volk or vkGetInstanceProcAddr needed!
    vkb::destroy_debug_utils_messenger(_instance, _debugMessenger);
    vkDestroyInstance(_instance, nullptr);
}

void Renderer::initVulkan() {
    // Create instance
    vkb::InstanceBuilder instanceBuilder;
    auto inst_ret = instanceBuilder.set_app_name("Sky Rendering Demo")
        .require_api_version(VK_API_VERSION_1_3)
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
    if(!SDL_Vulkan_CreateSurface(_window, _instance, nullptr, &_surface)) throw std::runtime_error("Failed to create surface via SDL!");

    // Pick physicalDevice based on compatibility with surface and extension support
    VkPhysicalDeviceVulkan13Features vulkan13RequiredFeatures{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
    vulkan13RequiredFeatures.dynamicRendering = VK_TRUE;

    vkb::PhysicalDeviceSelector selector{ vkb_inst };
    auto selector_ret = selector.set_surface(_surface)
        .set_minimum_version(1, 3)
        .set_required_features_13(vulkan13RequiredFeatures)
        .select();

    if(!selector_ret) throw std::runtime_error("Failed to find sufficient physicalDevice!");
    vkb::PhysicalDevice vkbPhysicalDevice = selector_ret.value();
    _physicalDevice = vkbPhysicalDevice.physical_device;

    // Create logical device and queues
    vkb::DeviceBuilder deviceBuilder{ vkbPhysicalDevice };
    auto dev_ret = deviceBuilder.build();
    if(!dev_ret) throw std::runtime_error("Failed to create Vulkan logical device!");
    vkb::Device vkb_device = dev_ret.value();
    _device = vkb_device.device;

    auto graphics_queue_ret = vkb_device.get_queue(vkb::QueueType::graphics);
    if(!graphics_queue_ret) throw std::runtime_error("Failed to find graphics queue!");
    _graphicsQueue = graphics_queue_ret.value();
    _graphicsFamilyIndex = vkb_device.get_queue_index(vkb::QueueType::graphics).value();
    
    auto present_queue_ret = vkb_device.get_queue(vkb::QueueType::present);
    if(!present_queue_ret) throw std::runtime_error("Failed to find present queue!"); /* Of couse present queue is not an actual queue type (see vkGetPhysicalDeviceSurfaceSupportKHR) */
    _presentQueue = vkb_device.get_queue(vkb::QueueType::present).value();
}