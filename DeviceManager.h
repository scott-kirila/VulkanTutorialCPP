//
// Created by hetan on 6/12/2024.
//

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "vulkan/vulkan.hpp"

struct QueueFamilyIndices
{
    bool IsComplete()
    {
        return m_GraphicsFamily.has_value();
    }

    std::optional<uint32_t> m_GraphicsFamily;
};

class DeviceManager {
public:
    void Destroy();

    void PickPhysicalDevice(vk::Instance& instance);

    void CreateLogicalDevice(const std::vector<const char *>& validationLayers);

private:
    QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device);


    bool IsDeviceSuitable(vk::PhysicalDevice device);

    vk::PhysicalDevice m_PhysicalDevice;
    vk::Queue m_GraphicsQueue;
    vk::Device m_LogicalDevice;
};



#endif //DEVICEMANAGER_H
