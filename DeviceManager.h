//
// Created by Scott Kirila on 6/12/2024.
//

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "GraphicsPipeline.h"
#include "vulkan/vulkan.hpp"

#include "SwapchainManager.h"

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

    void CreateCommandPool();

    void CreateCommandBuffer();

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
