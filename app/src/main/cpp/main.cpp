#define VK_USE_PLATFORM_ANDROID_KHR
#include <vulkan/vulkan.h>

#include <android_native_app_glue.h>
#include <android/log.h>
#include <stdlib.h>

#define LOG_TAG "VKRealNativeApp"
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


void android_main(android_app* app) {
}
