//
// Created by hetan on 6/10/2024.
//

#ifndef VKMANAGER_H
#define VKMANAGER_H

#include "DeviceManager.h"
#include "ValidationLayersManager.h"
#include "vulkan/vulkan.hpp"

class VkManager {
public:
    VkManager();
    ~VkManager();

    void Run();

    static std::vector<const char *> GetRequiredExtensions();

private:
    void CreateInstance();
    void CreateSwapchain();
    void CreateImageViews();
    void CreateGraphicsPipeline();

    std::string AppName = "Vulkan";
    std::string EngineName = "Vulkan.hpp";

    ValidationLayersManager m_ValidationManager;
    vk::Instance m_Instance;
    DeviceManager m_DeviceManager;
    vk::SwapchainKHR m_Swapchain; // Probably belongs in DeviceManager
    std::vector<vk::Image> m_SwapchainImages;
    std::vector<vk::ImageView> m_SwapchainImageViews;
    vk::Format m_SwapchainImageFormat{};
    vk::Extent2D m_SwapchainExtent{};
};



#endif //VKMANAGER_H
