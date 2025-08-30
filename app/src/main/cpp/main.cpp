#define VOLK_IMPLEMENTATION
#include "volk.h"

#include "logger.h"
#include "VulkanApp.h"

[[maybe_unused]]
void android_main(android_app* app) {
    auto vulkan_app= std::make_unique<VulkanApp>(app);
    vulkan_app->update();
}
