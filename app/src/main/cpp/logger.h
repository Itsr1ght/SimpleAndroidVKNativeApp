//
// Created by r1ght on 30/08/25.
//

#ifndef NATIVEAPP_LOGGER_H
#define NATIVEAPP_LOGGER_H

#include <android/log.h>

#define LOG_TAG "VKRealNativeApp"
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#endif //NATIVEAPP_LOGGER_H