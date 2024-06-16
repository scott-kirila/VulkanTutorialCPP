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
    void CreateSurface(const vk::Instance& instance);

    void PickPhysicalDevice(const vk::Instance& instance);

    void CreateLogicalDevice(const std::vector<const char *>& validationLayers);

    void CreatePipelineObjects();

    void Destroy() const;

    void Draw();

    WindowManager m_WindowManager;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_LogicalDevice;
    vk::SurfaceKHR m_Surface;
    GraphicsPipeline m_GraphicsPipeline;

private:
    [[nodiscard]] bool IsDeviceSuitable(const vk::PhysicalDevice& device) const;
};

#endif //DEVICEMANAGER_H
