//
// Created by r1ght on 30/08/25.
//

#include "VulkanApp.h"
#include "logger.h"

VulkanApp::VulkanApp(android_app *app) {
    auto result = volkInitialize();
    LOG_INFO("{volkInitialize} result : %s", VulkanApp::vkResultToString(result).c_str());
    this->app = app;
    this->create_instance();
    this->app->onAppCmd = VulkanApp::handle_cmd;
    this->app->userData = (void*)this;
}

void VulkanApp::create_instance() {
    const char* extensions[] = {
            "VK_KHR_surface",
            "VK_KHR_android_surface"
    };

    const char* layers[] = {"VK_LAYER_KHRONOS_validation"};

    auto application_info = VkApplicationInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Hello Android",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "Hello Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_2,
    };

    auto instance_create_info = VkInstanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &application_info,
        .enabledLayerCount = 1,
        .ppEnabledLayerNames = layers,
        .enabledExtensionCount = 2,
        .ppEnabledExtensionNames = extensions,
    };

    auto result = vkCreateInstance(&instance_create_info, nullptr, &this->instance);
    LOG_INFO("{vkCreateInstance} result : %s", VulkanApp::vkResultToString(result).c_str());
    volkLoadInstance(this->instance);
}

void VulkanApp::handle_cmd(struct android_app* app, int32_t cmd) {

    auto vulkan_app = static_cast<VulkanApp*>(app->userData);
    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {
            if (app->window != nullptr) {
                auto surface_create_info = VkAndroidSurfaceCreateInfoKHR{
                        .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
                        .pNext = NULL,
                        .flags = 0,
                        .window = app->window,
                };
                auto result = vkCreateAndroidSurfaceKHR(
                        vulkan_app->instance,
                        &surface_create_info,
                        nullptr,
                        &vulkan_app->surface
                );
                LOG_INFO("{vkCreateAndroidSurfaceKHR} result : %s", VulkanApp::vkResultToString(result).c_str());
                vulkan_app->update();
            }
            break;
        }
        case APP_CMD_DESTROY: {
            if(vulkan_app->instance && vulkan_app->surface)
                vkDestroySurfaceKHR(vulkan_app->instance, vulkan_app->surface, nullptr);
            if(vulkan_app->instance)
                vkDestroyInstance(vulkan_app->instance, nullptr);
            break;
        }
        default:{
            break;
        }
    }
}

void VulkanApp::update() {
    LOG_INFO("starting loop event");
    int events;
    android_poll_source* source;
    while (this->app->destroyRequested == 0){
        if (ALooper_pollOnce(
                0, nullptr,
                &events,
                (void**)&source) >= 0 && source != nullptr
                ){
            if(source != nullptr) {
                source->process(this->app, source);
            }
        }
    }
}

std::string VulkanApp::vkResultToString(VkResult result){
    switch (result) {
        case VK_SUCCESS: return "VK_SUCCESS";
        case VK_NOT_READY: return "VK_NOT_READY";
        case VK_TIMEOUT: return "VK_TIMEOUT";
        case VK_EVENT_SET: return "VK_EVENT_SET";
        case VK_EVENT_RESET: return "VK_EVENT_RESET";
        case VK_INCOMPLETE: return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
            // add more from vk_result.h if needed
        default: return "VK_RESULT_UNDEFINED";
    }
}

const char* VulkanApp::get_asset_data(const char* asset_name){
    AAssetManager* assetManager = this->app->activity->assetManager;
    AAsset* asset = AAssetManager_open(assetManager, asset_name, AASSET_MODE_BUFFER);
    if (!asset) {
        LOG_ERROR("Failed to open asset");
        return nullptr;
    } else {
        const void* buffer = AAsset_getBuffer(asset);
        AAsset_close(asset);
        return static_cast<const char*>(buffer);
    }
}

void VulkanApp::_update() {

}
