//
// Created by hetan on 6/10/2024.
//

#ifndef VALIDATIONLAYERSMANAGER_H
#define VALIDATIONLAYERSMANAGER_H

#include <vector>

#include "vulkan/vulkan.hpp"

struct ValidationLayersManager {

    const std::vector<const char*> m_ValidationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

    bool CheckValidationLayerSupport() const;

    // void SetupDebugMessenger(vk::Instance &instance, vk::DebugUtilsMessengerEXT debugMessenger);
    //
    // static VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    //                               VkDebugUtilsMessageTypeFlagsEXT messageType,
    //                               const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

    // vk::DebugUtilsMessengerCreateInfoEXT GenerateDebugMessengerCreateInfo();
};

#endif //VALIDATIONLAYERSMANAGER_H
