cmake_minimum_required(VERSION 3.22.1)
project(native_app LANGUAGES C CXX)

include(${ANDROID_NDK}/build/cmake/android.toolchain.cmake)
include(./cmake/CPM.cmake)

add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR=1)

# Set C++ standard to match the main project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

CPMAddPackage(
    URI "gh:KhronosGroup/Vulkan-Headers#sdk-1.2.198"
)

CPMAddPackage(
    URI "gh:zeux/volk#1.2.198"
)

add_library(native_app SHARED
    main.cpp
    VulkanApp.cpp
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(native_app PRIVATE DEBUG=1)
    target_compile_definitions(native_app PRIVATE NDEBUG=1)
    target_compile_definitions(native_app PRIVATE VK_DEBUG_BUILD=1)
    message(STATUS "Configuring DEBUG build")
else()
    target_compile_definitions(native_app PRIVATE VK_RELEASE_BUILD=1)
    message(STATUS "Configuring RELEASE build")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_include_directories(native_app PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue
        Vulkan-Headers
)

target_link_libraries(native_app
    PRIVATE
    android
    log
    volk::volk
)
