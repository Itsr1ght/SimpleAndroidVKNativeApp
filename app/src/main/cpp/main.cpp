#define VOLK_IMPLEMENTATION
#include "volk.h"

#include <android_native_app_glue.h>
#include <android/log.h>
#include <cstdlib>

#define LOG_TAG "VKRealNativeApp"
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


struct VulkanData{
    VkInstance instance;
};

VulkanData data;

void android_main(android_app* app) {

    if (volkInitialize() == VK_FALSE){
	LOG_ERROR("Cannot initialize volk");
    }
    
    auto instance_create_info = VkInstanceCreateInfo {
    };
    vkCreateInstance(&instance_create_info, nullptr, &data.instance);

}
