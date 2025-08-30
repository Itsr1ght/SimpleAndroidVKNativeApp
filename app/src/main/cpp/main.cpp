#define VOLK_IMPLEMENTATION
#include "volk.h"

#include "vulkan/vulkan.h"
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
#include <android/log.h>

#include <memory>
#include <vector>
#include <cstdlib>
#include <string>

#define LOG_TAG "VKRealNativeApp"
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


class VulkanApp {
public:
    explicit VulkanApp(android_app* app);
    void static handle_cmd(struct android_app* app, int32_t cmd);
    static std::string vkResultToString(VkResult result);
    void create_instance();
    void update();

private:
    const char* get_asset_data(const char* asset_name);

    android_app* app;
    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
};

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
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
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
    switch(result){
        case VK_SUCCESS: return {"VK_SUCCESS"};
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return {"VK_ERROR_NATIVE_WINDOW_IN_USE_KHR"};
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return {"VK_ERROR_OUT_OF_DEVICE_MEMORY"};
        case VK_ERROR_OUT_OF_HOST_MEMORY: return {"VK_ERROR_OUT_OF_HOST_MEMORY"};
        case VK_ERROR_UNKNOWN: return {"VK_ERROR_UNKNOWN"};
        case VK_ERROR_VALIDATION_FAILED_EXT: return {"VK_ERROR_VALIDATION_FAILED"};
        default: return {"ERROR_UNKNOWN"};
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

[[maybe_unused]]
void android_main(android_app* app) {
    auto vulkan_app= std::make_unique<VulkanApp>(app);
    vulkan_app->update();
}
