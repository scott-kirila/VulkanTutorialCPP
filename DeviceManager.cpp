//
// Created by hetan on 6/12/2024.
//

#include "DeviceManager.h"

#include <set>

void DeviceManager::Destroy() const
{
    m_LogicalDevice.destroy();
}

QueueFamilyIndices DeviceManager::FindQueueFamilies(const vk::PhysicalDevice device) const
{
    QueueFamilyIndices indices;

    const std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.m_GraphicsFamily = i;
        }

        if (device.getSurfaceSupportKHR(i, m_Surface))
        {
            indices.m_PresentFamily = i;
        }

        if (indices.IsComplete()) break;

        i++;
    }

    return indices;
}

void DeviceManager::PickPhysicalDevice(const vk::Instance &instance)
{
    const std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    if (physicalDevices.empty())
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support.");
    }

    for (const auto& device: physicalDevices)
    {
        if (IsDeviceSuitable(device))
        {
            m_PhysicalDevice = device;
            break;
        }
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Failed to find a suitable GPU.");
    }
}

void DeviceManager::CreateLogicalDevice(const std::vector<const char*>& validationLayers)
{
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamiles = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamiles)
    {
        queueCreateInfos.emplace_back(
            vk::DeviceQueueCreateInfo(
                {},
                queueFamily,
                1,
                &queuePriority
            )
        );
    }

    vk::PhysicalDeviceFeatures physicalDeviceFeatures{};
    auto createInfo = vk::DeviceCreateInfo(
        {},
        queueCreateInfos,
        {},
        m_SwapchainManager.m_DeviceExtensions,
        &physicalDeviceFeatures
    );

    #ifndef NDEBUG
    createInfo.setEnabledLayerCount(validationLayers.size());
    createInfo.setPpEnabledLayerNames(validationLayers.data());
    #endif

    if (m_PhysicalDevice.createDevice(&createInfo, nullptr, &m_LogicalDevice) != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to create logical device");
    }

    m_LogicalDevice.getQueue(indices.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);
    m_LogicalDevice.getQueue(indices.m_PresentFamily.value(), 0, &m_PresentQueue);
}

void DeviceManager::CreateSurface(vk::Instance instance)
{
    if (glfwCreateWindowSurface(instance, m_WindowManager.m_Window, nullptr,
        reinterpret_cast<VkSurfaceKHR*>(&m_Surface)) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface.");
        }
}

void DeviceManager::CreateSwapchain()
{
    auto swapchainSupport = SwapchainManager::QuerySwapchainSupport(m_PhysicalDevice, m_Surface);

    vk::SurfaceFormatKHR surfaceFormat = SwapchainManager::ChooseSwapSurfaceFormat(swapchainSupport.m_Formats);
    vk::PresentModeKHR presentMode = SwapchainManager::ChooseSwapPresentMode(swapchainSupport.m_PresentModes);
    vk::Extent2D extent = SwapchainManager::ChooseSwapExtent(swapchainSupport.m_Capabilities, m_WindowManager);

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

    auto indices = FindQueueFamilies(m_PhysicalDevice);
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

    if (m_LogicalDevice.createSwapchainKHR(&createInfo, nullptr, &m_SwapchainManager.m_Swapchain)
        != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to create swapchain.");
    }

    m_SwapchainManager.m_SwapchainImages = m_LogicalDevice.getSwapchainImagesKHR(m_SwapchainManager.m_Swapchain);

    m_SwapchainManager.m_SwapchainImageFormat = surfaceFormat.format;
    m_SwapchainManager.m_SwapchainExtent = extent;
}

void DeviceManager::CreateImageViews()
{
    m_SwapchainManager.m_SwapchainImageViews.resize(m_SwapchainManager.m_SwapchainImages.size());

    for (size_t i = 0; i < m_SwapchainManager.m_SwapchainImages.size(); i++)
    {
        auto createInfo = vk::ImageViewCreateInfo(
            {},
            m_SwapchainManager.m_SwapchainImages[i],
            vk::ImageViewType::e2D,
            m_SwapchainManager.m_SwapchainImageFormat,
            {
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity },
            {
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1 }
        );

        if (m_LogicalDevice.createImageView(&createInfo, nullptr, &m_SwapchainManager.m_SwapchainImageViews[i])
            != vk::Result::eSuccess)
        {
            throw std::runtime_error("Failed to create image views.");
        }
    }
}

bool DeviceManager::IsDeviceSuitable(const vk::PhysicalDevice device) const
{
    const QueueFamilyIndices indices = FindQueueFamilies(device);

    const bool extensionsSupported = m_SwapchainManager.CheckDeviceExtensionSupport(device);
    bool swapchainAdequate = false;

    if (extensionsSupported)
    {
        auto swapchainSupport = SwapchainManager::QuerySwapchainSupport(device, m_Surface);
        swapchainAdequate = !swapchainSupport.m_Formats.empty() && !swapchainSupport.m_PresentModes.empty();
    }

    return indices.IsComplete() && swapchainAdequate;
}
