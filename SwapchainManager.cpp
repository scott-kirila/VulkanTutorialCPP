//
// Created by Scott Kirila on 6/13/2024.
//

#include "SwapchainManager.h"

#include <set>

#include "Utilities.h"

bool SwapchainManager::CheckDeviceExtensionSupport(const vk::PhysicalDevice& device) const
{
    const std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();
    std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapchainSupportDetails SwapchainManager::QuerySwapchainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
{
    auto details = SwapchainSupportDetails(
        device.getSurfaceCapabilitiesKHR(surface),
        device.getSurfaceFormatsKHR(surface),
        device.getSurfacePresentModesKHR(surface)
    );

    return details;
}

vk::SurfaceFormatKHR SwapchainManager::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
            availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR SwapchainManager::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
            return availablePresentMode;
        }
    }

    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D SwapchainManager::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities, const WindowManager &windowManager)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        windowManager.GetFramebufferSize(width, height);

        vk::Extent2D actualExtent{
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width,
            capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height,
            capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void SwapchainManager::CreateSwapchain(const vk::PhysicalDevice& physicalDevice, const vk::Device& logicalDevice,
    const vk::SurfaceKHR& surface, const WindowManager& windowManager)
{
    auto swapchainSupport = QuerySwapchainSupport(physicalDevice, surface);

    const vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapchainSupport.m_Formats);
    const vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapchainSupport.m_PresentModes);
    const vk::Extent2D extent = ChooseSwapExtent(swapchainSupport.m_Capabilities, windowManager);

    uint32_t imageCount = swapchainSupport.m_Capabilities.minImageCount + 1;

    if (swapchainSupport.m_Capabilities.maxImageCount > 0 && imageCount > swapchainSupport.m_Capabilities.maxImageCount)
    {
        imageCount = swapchainSupport.m_Capabilities.maxImageCount;
    }

    auto createInfo = vk::SwapchainCreateInfoKHR(
        {},
        surface,
        imageCount,
        surfaceFormat.format,
        surfaceFormat.colorSpace,
        extent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment
    );

    const auto indices = Utilities::FindQueueFamilies(physicalDevice, surface);
    const uint32_t queueFamilyIndices[] = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

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

    if (logicalDevice.createSwapchainKHR(&createInfo, nullptr, &m_Swapchain)
        != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to create swapchain.");
    }

    m_SwapchainImages = logicalDevice.getSwapchainImagesKHR(m_Swapchain);

    m_SwapchainImageFormat = surfaceFormat.format;
    m_SwapchainExtent = extent;
}

void SwapchainManager::CleanupSwapchain(const vk::Device &logicalDevice) const
{
    for (const auto imageView : m_SwapchainImageViews)
    {
        logicalDevice.destroyImageView(imageView);
    }

    logicalDevice.destroySwapchainKHR(m_Swapchain);
}

void SwapchainManager::CreateImageViews(const vk::Device& logicalDevice)
{
    m_SwapchainImageViews.resize(m_SwapchainImages.size());

    for (size_t i = 0; i < m_SwapchainImages.size(); i++)
    {
        auto createInfo = vk::ImageViewCreateInfo(
            {},
            m_SwapchainImages[i],
            vk::ImageViewType::e2D,
            m_SwapchainImageFormat,
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

        if (logicalDevice.createImageView(&createInfo, nullptr,
            &m_SwapchainImageViews[i])
            != vk::Result::eSuccess)
        {
            throw std::runtime_error("Failed to create image views.");
        }
    }
}
