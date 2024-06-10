//
// Created by hetan on 6/10/2024.
//

#include "VkManager.h"

#include <iostream>

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
    uint32_t glfwExtensionCount{};
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    std::vector<vk::ExtensionProperties> vkExtensions = vk::enumerateInstanceExtensionProperties();

    for (const auto &extension: vkExtensions)
    {
        std::cout << extension.extensionName << std::endl;
    }

    vk::ApplicationInfo appInfo{
        AppName.c_str(),
        1,
        EngineName.c_str(),
        1,
        VK_API_VERSION_1_3,
    };

    vk::InstanceCreateInfo createInfo{
        vk::InstanceCreateFlags(),
        &appInfo,
        {},
        {},
        static_cast<uint32_t>(extensions.size()),
        extensions.data(),
    };

    if (vk::createInstance(&createInfo, nullptr, &m_Instance) != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to create Vulkan instance.");
    }
}

void VkManager::Run()
{
    m_WindowManager.DoLoop();
}
