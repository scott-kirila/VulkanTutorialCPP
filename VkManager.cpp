//
// Created by hetan on 6/10/2024.
//
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include "VkManager.h"

#include <iostream>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VkManager::VkManager()
{
    CreateInstance();
}

VkManager::~VkManager()
{
    m_Instance.destroy();
}

void VkManager::CreateInstance()
{
    // This (apparently) MUST be called before other Vulkan functions!
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    #ifndef NDEBUG
    if (!m_ValidationManager.CheckValidationLayerSupport())
    {
        throw std::runtime_error("Validation layers requestion but unavailable.");
    }
    #endif

    std::vector<vk::ExtensionProperties> vkExtensions = vk::enumerateInstanceExtensionProperties();
    for (const auto& vkExtension: vkExtensions)
    {
        std::cout << vkExtension.extensionName << std::endl;
    }

    auto extensions = GetRequiredExtensions();

    vk::ApplicationInfo appInfo(
        AppName.c_str(),
        1,
        EngineName.c_str(),
        1,
        VK_API_VERSION_1_3);

    vk::InstanceCreateInfo createInfo{
        {},
        &appInfo,
        {},
        {},
        static_cast<uint32_t>(extensions.size()),
        extensions.data(),
    };

    #ifndef NDEBUG
    createInfo.enabledLayerCount = m_ValidationManager.m_ValidationLayers.size();
    createInfo.ppEnabledLayerNames = m_ValidationManager.m_ValidationLayers.data();
    #endif

    m_Instance = vk::createInstance(createInfo);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_Instance);
}

void VkManager::Run()
{
    m_WindowManager.DoLoop();
}

std::vector<const char *> VkManager::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount{};
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    #ifndef NDEBUG
    extensions.push_back(vk::EXTDebugUtilsExtensionName);
    #endif

    return extensions;
}