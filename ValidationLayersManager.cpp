//
// Created by hetan on 6/10/2024.
//

#include "ValidationLayersManager.h"

#include <iostream>

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    if (const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT"));
        func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator)
{
    if (const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
        func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

ValidationLayersManager::ValidationLayersManager()
{
    m_DebugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT(
        vk::DebugUtilsMessengerCreateFlagsEXT(),
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
        DebugCallback,
        nullptr
    );
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

void ValidationLayersManager::SetupDebugMessenger(const vk::Instance& instance)
{
    if (CreateDebugUtilsMessengerEXT(instance,
        &m_DebugCreateInfo,
        nullptr,
        &m_DebugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug callback!");
    }
}

VkBool32 ValidationLayersManager::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

vk::DebugUtilsMessengerCreateInfoEXT ValidationLayersManager::GenerateDebugMessengerCreateInfo()
{
    auto severity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;

    auto type = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
        | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
        | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

    return { vk::DebugUtilsMessengerCreateFlagsEXT(), severity, type, DebugCallback, nullptr };
};
