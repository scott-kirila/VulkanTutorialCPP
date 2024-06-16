//
// Created by Scott Kirila on 6/13/2024.
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

struct SwapchainManager {
    [[nodiscard]] bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device) const;

    [[nodiscard]] static SwapchainSupportDetails QuerySwapchainSupport(const vk::PhysicalDevice& device,
        const vk::SurfaceKHR& surface);

    static vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

    static vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

    static vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities,
        const WindowManager& windowManager);

    void CreateSwapchain(const vk::PhysicalDevice& physicalDevice, const vk::Device& logicalDevice,
                         const vk::SurfaceKHR& surface, const WindowManager& windowManager);

    void CleanupSwapchain(const vk::Device& logicalDevice) const;

    void CreateImageViews(const vk::Device& logicalDevice);

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
