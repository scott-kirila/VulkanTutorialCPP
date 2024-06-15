//
// Created by Scott Kirila on 6/12/2024.
//

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "GraphicsPipeline.h"
#include "vulkan/vulkan.hpp"

#include "SwapchainManager.h"

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

    void PickPhysicalDevice(const vk::Instance& instance);

    void CreateLogicalDevice(const std::vector<const char *>& validationLayers);

    void CreateSurface(const vk::Instance& instance);

    void CreateSwapchain();

    void CreateImageViews();

    void CreateRenderPass();

    void CreateGraphicsPipeline();

    void CreateFramebuffers();

    [[nodiscard]] QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device) const;

    WindowManager m_WindowManager;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_LogicalDevice;
    vk::SurfaceKHR m_Surface;
    GraphicsPipeline m_GraphicsPipeline;
    vk::Queue m_GraphicsQueue;
    vk::Queue m_PresentQueue;

private:

    [[nodiscard]] bool IsDeviceSuitable(const vk::PhysicalDevice& device) const;
};

#endif //DEVICEMANAGER_H
