//
// Created by hetan on 6/10/2024.
//

#include "ValidationLayersManager.h"

#include <iostream>

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator,
                                      VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

bool ValidationLayersManager::CheckValidationLayerSupport() const
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    for (const auto& layer : m_ValidationLayers)
    {
        auto findMatch = [layer](vk::LayerProperties layerProperties)
        {
            return strcmp(layer, layerProperties.layerName) == 0;
        };

        if (std::ranges::find_if(availableLayers, findMatch) == availableLayers.end()) return false;
    }

    return true;
}
//
// void ValidationLayersManager::SetupDebugMessenger(vk::Instance& instance, vk::DebugUtilsMessengerEXT debugMessenger)
// {
//     auto createInfo = GenerateDebugMessengerCreateInfo();
//
//     if (instance.createDebugUtilsMessengerEXT(&createInfo, nullptr, &debugMessenger) != vk::Result::eSuccess)
//     {
//         throw std::runtime_error("Failed to set up debug callback.");
//     }
// }
//
// VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayersManager::DebugCallback(
//     VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
//     VkDebugUtilsMessageTypeFlagsEXT messageType,
//     const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
//     void *pUserData)
// {
//     std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
//
//     return VK_FALSE;
// }
//
// vk::DebugUtilsMessengerCreateInfoEXT ValidationLayersManager::GenerateDebugMessengerCreateInfo()
// {
//     return {
//         vk::DebugUtilsMessengerCreateFlagsEXT(),
//         vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
//         | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
//         | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
//         vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
//         | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
//         | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
//         DebugCallback,
//         nullptr
//     };
// }

