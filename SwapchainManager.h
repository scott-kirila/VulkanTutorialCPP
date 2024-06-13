//
// Created by hetan on 6/13/2024.
//

#ifndef SWAPCHAINMANAGER_H
#define SWAPCHAINMANAGER_H

#include "vulkan/vulkan.hpp"

#include "WindowManager.h"

struct SwapchainSupportDetails
{
    vk::SurfaceCapabilitiesKHR m_Capabilities;
    std::vector<vk::SurfaceFormatKHR> m_Formats;
    std::vector<vk::PresentModeKHR> m_PresentModes;
};

class SwapchainManager {
public:
    [[nodiscard]] bool CheckDeviceExtensionSupport(vk::PhysicalDevice device) const;

    [[nodiscard]] static SwapchainSupportDetails QuerySwapchainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface);

    // Swapchain settings
    static vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

    static vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

    static vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const WindowManager& windowManager);

    const std::vector<const char*> m_DeviceExtensions = {
        vk::KHRSwapchainExtensionName,
    };

    vk::SwapchainKHR m_Swapchain;
    std::vector<vk::Image> m_SwapchainImages;
    std::vector<vk::ImageView> m_SwapchainImageViews;
    vk::Format m_SwapchainImageFormat{};
    vk::Extent2D m_SwapchainExtent{};
};

#endif //SWAPCHAINMANAGER_H
