//
// Created by hetan on 6/10/2024.
//

#ifndef VALIDATIONLAYERSMANAGER_H
#define VALIDATIONLAYERSMANAGER_H

#include <vector>

#include "vulkan/vulkan.hpp"

struct ValidationLayersManager {
    ValidationLayersManager();

    [[nodiscard]] bool CheckValidationLayerSupport() const;

    void SetupDebugMessenger(vk::Instance &instance);

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    vk::DebugUtilsMessengerCreateInfoEXT GenerateDebugMessengerCreateInfo();

    const std::vector<const char*> m_ValidationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

    VkDebugUtilsMessengerEXT m_DebugMessenger;
    VkDebugUtilsMessengerCreateInfoEXT m_DebugCreateInfo;
};

#endif //VALIDATIONLAYERSMANAGER_H
