//
// Created by hetan on 6/12/2024.
//

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "WindowManager.h"
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

struct SwapchainSupportDetails
{
    vk::SurfaceCapabilitiesKHR m_Capabilities;
    std::vector<vk::SurfaceFormatKHR> m_Formats;
    std::vector<vk::PresentModeKHR> m_PresentModes;
};

class DeviceManager {
public:
    void Destroy() const;

    void PickPhysicalDevice(const vk::Instance& instance);

    void CreateLogicalDevice(const std::vector<const char *>& validationLayers);

    void CreateSurface(vk::Instance instance);

    SwapchainSupportDetails QuerySwapchainSupport(vk::PhysicalDevice device);

    // Swapchain settings
    vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

    vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

    QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device);

    const std::vector<const char*> m_DeviceExtensions = {
        vk::KHRSwapchainExtensionName,
    };

    WindowManager m_WindowManager;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_LogicalDevice;
    vk::SurfaceKHR m_Surface;
    vk::Queue m_GraphicsQueue;
    vk::Queue m_PresentQueue;

private:

    bool IsDeviceSuitable(vk::PhysicalDevice device);

    bool CheckDeviceExtensionSupport(vk::PhysicalDevice device);


};



#endif //DEVICEMANAGER_H
