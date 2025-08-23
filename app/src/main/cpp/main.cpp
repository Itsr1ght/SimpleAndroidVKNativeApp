#define VOLK_IMPLEMENTATION
#include "volk.h"

#include "vulkan/vulkan.h"
#include <android_native_app_glue.h>
#include <android/log.h>
#include <cstdlib>
#include <string>

#define LOG_TAG "VKRealNativeApp"
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


struct VulkanData{
    VkInstance instance;
    VkSurfaceKHR surface;
};

VulkanData data;

std::string vkResultToString(VkResult result){
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

void handle_cmd(struct android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:{
            if (app->window != nullptr){
                auto surface_create_info = VkAndroidSurfaceCreateInfoKHR{
                        .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
                        .pNext = NULL,
                        .flags = 0,
                        .window = app->window,
                };
                auto result = vkCreateAndroidSurfaceKHR(
                        data.instance,
                        &surface_create_info,
                        nullptr,
                        &data.surface
                );

                LOG_INFO("{vkCreateAndroidSurfaceKHR} result : %s", vkResultToString(result).c_str());
                break;
            }
        }
        case APP_CMD_DESTROY:{
            vkDestroyInstance(data.instance, nullptr);
            break;
        }
        default:{
            break;
        }
    }
}

[[maybe_unused]]
void android_main(android_app* app) {
    app->onAppCmd = handle_cmd;

    if (volkInitialize() != VK_SUCCESS) {
        LOG_ERROR("Cannot initialize VOLK");
    }

    LOG_INFO("initialize VOLK Successfully");

    uint32_t version = 0;
    vkEnumerateInstanceVersion(&version);
    LOG_INFO("Vulkan version supported: %u.%u.%u",
         VK_VERSION_MAJOR(version),
         VK_VERSION_MINOR(version),
         VK_VERSION_PATCH(version)
    );

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
        .enabledExtensionCount = 2,
        .ppEnabledExtensionNames = extensions,
    };
    vkCreateInstance(&instance_create_info, nullptr, &data.instance);

    volkLoadInstance(data.instance);

    LOG_INFO("starting loop event");
    int events;
    android_poll_source* source;
    while (app->destroyRequested == 0){
        if (ALooper_pollOnce(0, nullptr, &events, (void**)&source) >= 0 && source != nullptr){
            if(source != nullptr) {
                source->process(app, source);
            }
        }
    }
    LOG_INFO("loop event ended");
}
