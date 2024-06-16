//
// Created by Scott Kirila on 6/10/2024.
//

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include "VkManager.h"

#include <iostream>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VkManager::VkManager()
{
    CreateInstance();
    #ifndef NDEBUG
    m_ValidationManager.SetupDebugMessenger(m_Instance);
    #endif
    m_DeviceManager.CreateSurface(m_Instance);
    m_DeviceManager.PickPhysicalDevice(m_Instance);
    m_DeviceManager.CreateLogicalDevice(m_ValidationManager.m_ValidationLayers);
    m_DeviceManager.CreatePipelineObjects();
}

VkManager::~VkManager()
{
    m_DeviceManager.Destroy();

    #ifndef NDEBUG
    m_Instance.destroyDebugUtilsMessengerEXT(m_ValidationManager.m_DebugMessenger);
    #endif
    m_Instance.destroySurfaceKHR(m_DeviceManager.m_Surface);
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
    createInfo.pNext = &m_ValidationManager.m_DebugCreateInfo;
    #endif

    m_Instance = vk::createInstance(createInfo);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_Instance);
}

void VkManager::Run()
{
    m_DeviceManager.Draw();
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