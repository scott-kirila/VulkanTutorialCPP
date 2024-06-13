//
// Created by hetan on 6/10/2024.
//
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include "VkManager.h"

#include <iostream>

// #define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VkManager::VkManager()
{
    CreateInstance();
    #ifndef NDEBUG
    m_ValidationManager.SetupDebugMessenger(m_Instance);
    #endif
    CreateSurface();
    m_DeviceManager.PickPhysicalDevice(m_Instance, m_Surface);
    m_DeviceManager.CreateLogicalDevice(m_ValidationManager.m_ValidationLayers, m_Surface);
    CreateSwapchain();
}

VkManager::~VkManager()
{
    m_DeviceManager.m_LogicalDevice.destroySwapchainKHR(m_Swapchain);
    m_DeviceManager.Destroy();
    #ifndef NDEBUG
    m_Instance.destroyDebugUtilsMessengerEXT(m_ValidationManager.m_DebugMessenger);
    #endif
    m_Instance.destroySurfaceKHR(m_Surface);
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

void VkManager::CreateSurface()
{
    if (glfwCreateWindowSurface(m_Instance, m_DeviceManager.m_WindowManager.m_Window, nullptr,
        reinterpret_cast<VkSurfaceKHR*>(&m_Surface)) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void VkManager::CreateSwapchain()
{
    auto swapchainSupport = m_DeviceManager.QuerySwapchainSupport(m_DeviceManager.m_PhysicalDevice, m_Surface);

    vk::SurfaceFormatKHR surfaceFormat = m_DeviceManager.ChooseSwapSurfaceFormat(swapchainSupport.m_Formats);
    vk::PresentModeKHR presentMode = m_DeviceManager.ChooseSwapPresentMode(swapchainSupport.m_PresentModes);
    vk::Extent2D extent = m_DeviceManager.ChooseSwapExtent(swapchainSupport.m_Capabilities);

    uint32_t imageCount = swapchainSupport.m_Capabilities.minImageCount + 1;

    if (swapchainSupport.m_Capabilities.maxImageCount > 0 && imageCount > swapchainSupport.m_Capabilities.maxImageCount)
    {
        imageCount = swapchainSupport.m_Capabilities.maxImageCount;
    }

    auto createInfo = vk::SwapchainCreateInfoKHR(
        {},
        m_Surface,
        imageCount,
        surfaceFormat.format,
        surfaceFormat.colorSpace,
        extent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment
    );

    auto indices = m_DeviceManager.FindQueueFamilies(m_DeviceManager.m_PhysicalDevice, m_Surface);
    uint32_t queueFamilyIndices[] = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

    if (indices.m_GraphicsFamily != indices.m_PresentFamily)
    {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    }

    createInfo.preTransform = swapchainSupport.m_Capabilities.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = presentMode;
    createInfo.clipped = vk::True;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (m_DeviceManager.m_LogicalDevice.createSwapchainKHR(&createInfo, nullptr, &m_Swapchain)
        != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to create swapchain.");
    }

    m_SwapchainImages = m_DeviceManager.m_LogicalDevice.getSwapchainImagesKHR(m_Swapchain);

    m_SwapchainImageFormat = surfaceFormat.format;
    m_SwapchainExtent = extent;
}

void VkManager::Run()
{
    m_DeviceManager.m_WindowManager.DoLoop();
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