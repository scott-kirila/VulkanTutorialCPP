//
// Created by hetan on 6/12/2024.
//

#include "DeviceManager.h"

void DeviceManager::Destroy()
{
    m_LogicalDevice.destroy();
}

QueueFamilyIndices DeviceManager::FindQueueFamilies(vk::PhysicalDevice device)
{
    QueueFamilyIndices indices;

    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.m_GraphicsFamily = i;
        }

        if (indices.IsComplete()) break;

        i++;
    }



    return indices;
}

void DeviceManager::PickPhysicalDevice(vk::Instance &instance)
{
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

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

    float queuePriority = 1.0f;
    auto queueCreateInfo = vk::DeviceQueueCreateInfo(
        {},
        indices.m_GraphicsFamily.value(),
        1,
        &queuePriority
    );

    vk::PhysicalDeviceFeatures physicalDeviceFeatures{};
    auto createInfo = vk::DeviceCreateInfo(
        {},
        queueCreateInfo,
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
}

bool DeviceManager::IsDeviceSuitable(vk::PhysicalDevice device)
{
    QueueFamilyIndices indices = FindQueueFamilies(device);

    return indices.IsComplete();
}
