//
// Created by hetan on 6/12/2024.
//

#include "DeviceManager.h"

#include <set>
// #include <GLFW/glfw3.h>

void DeviceManager::Destroy() const
{
    m_LogicalDevice.destroy();
}

QueueFamilyIndices DeviceManager::FindQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface)
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

        if (device.getSurfaceSupportKHR(i, surface))
        {
            indices.m_PresentFamily = i;
        }

        if (indices.IsComplete()) break;

        i++;
    }

    return indices;
}

void DeviceManager::PickPhysicalDevice(const vk::Instance &instance, const vk::SurfaceKHR surface)
{
    const std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    if (physicalDevices.empty())
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support.");
    }

    for (const auto& device: physicalDevices)
    {
        if (IsDeviceSuitable(device, surface))
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

void DeviceManager::CreateLogicalDevice(const std::vector<const char*>& validationLayers, vk::SurfaceKHR surface)
{
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice, surface);

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
        m_DeviceExtensions,
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

bool DeviceManager::IsDeviceSuitable(const vk::PhysicalDevice device, const vk::SurfaceKHR surface)
{
    const QueueFamilyIndices indices = FindQueueFamilies(device, surface);

    const bool extensionsSupported = CheckDeviceExtensionSupport(device);
    bool swapchainAdequate = false;

    if (extensionsSupported)
    {
        auto swapchainSupport = QuerySwapchainSupport(device, surface);
        swapchainAdequate = !swapchainSupport.m_Formats.empty() && !swapchainSupport.m_PresentModes.empty();
    }

    return indices.IsComplete() && swapchainAdequate;
}

bool DeviceManager::CheckDeviceExtensionSupport(vk::PhysicalDevice device)
{
    std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();
    std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapchainSupportDetails DeviceManager::QuerySwapchainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface)
{
    // SwapchainSupportDetails details;
    // details.m_Capabilities = device.getSurfaceCapabilitiesKHR(surface);
    // details.m_Formats = device.getSurfaceFormatsKHR(surface);
    // details.m_PresentModes = device.getSurfacePresentModesKHR(surface);
    auto details = SwapchainSupportDetails(
        device.getSurfaceCapabilitiesKHR(surface),
        device.getSurfaceFormatsKHR(surface),
        device.getSurfacePresentModesKHR(surface)
    );

    return details;
}

vk::SurfaceFormatKHR DeviceManager::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats)
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

vk::PresentModeKHR DeviceManager::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes)
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

vk::Extent2D DeviceManager::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        m_WindowManager.GetFramebufferSize(width, height);
        // glfwGetFramebufferSize(windowManager.m_Window, &width, &height);

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
