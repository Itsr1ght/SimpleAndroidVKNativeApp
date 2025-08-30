//
// Created by r1ght on 30/08/25.
//

//VOLK HEADER
#include "volk.h"

// ANDROID AND VULKAN HEADERS
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
#include <android/log.h>

// CPP HEADERS
#include <memory>
#include <vector>
#include <cstdlib>
#include <string>



#ifndef NATIVEAPP_VULKANAPP_H
#define NATIVEAPP_VULKANAPP_H

class VulkanApp {
public:
    explicit VulkanApp(android_app* app);
    void update();

private:
    void _update();
    void static handle_cmd(struct android_app* app, int32_t cmd);
    static std::string vkResultToString(VkResult result);
    const char* get_asset_data(const char* asset_name);
    void create_instance();

    android_app* app;
    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
};

#endif //NATIVEAPP_VULKANAPP_H
