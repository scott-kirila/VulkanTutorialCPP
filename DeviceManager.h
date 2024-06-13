//
// Created by hetan on 6/12/2024.
//

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "vulkan/vulkan.hpp"

struct QueueFamilyIndices
{
    [[nodiscard]] bool IsComplete() const
    {
        return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
    }

    std::optional<uint32_t> m_GraphicsFamily;
    std::optional<uint32_t> m_PresentFamily;
};

class DeviceManager {
public:
    void Destroy() const;

    void PickPhysicalDevice(const vk::Instance& instance, vk::SurfaceKHR surface);

    void CreateLogicalDevice(const std::vector<const char *>& validationLayers, vk::SurfaceKHR surface);

    vk::Queue m_GraphicsQueue;
    vk::Queue m_PresentQueue;

private:
    static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface);


    static bool IsDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface);

    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_LogicalDevice;
};



#endif //DEVICEMANAGER_H
