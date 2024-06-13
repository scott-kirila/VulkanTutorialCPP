//
// Created by hetan on 6/12/2024.
//

#include "DeviceManager.h"

#include <set>

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
        {},
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

    return indices.IsComplete();
}
