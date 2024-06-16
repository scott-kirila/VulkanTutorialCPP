//
// Created by Scott Kirila on 6/12/2024.
//

#include "DeviceManager.h"

#include <set>

#include "Utilities.h"

void DeviceManager::PickPhysicalDevice(const vk::Instance &instance)
{
    const std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

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
    QueueFamilyIndices indices = Utilities::FindQueueFamilies(m_PhysicalDevice, m_Surface);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamiles = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamiles)
    {
        queueCreateInfos.emplace_back(
            vk::DeviceQueueCreateInfo(
                {},
                queueFamily,
                1,
                &queuePriority
            )
        );
    }

    vk::PhysicalDeviceFeatures physicalDeviceFeatures{};
    auto createInfo = vk::DeviceCreateInfo(
        {},
        queueCreateInfos,
        {},
        m_GraphicsPipeline.m_SwapchainManager.m_DeviceExtensions,
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

    m_LogicalDevice.getQueue(indices.m_GraphicsFamily.value(), 0, &m_GraphicsPipeline.m_GraphicsQueue);
    m_LogicalDevice.getQueue(indices.m_PresentFamily.value(), 0, &m_GraphicsPipeline.m_PresentQueue);
}

void DeviceManager::CreateSurface(const vk::Instance& instance)
{
    if (glfwCreateWindowSurface(instance, m_WindowManager.m_Window, nullptr,
        reinterpret_cast<VkSurfaceKHR*>(&m_Surface)) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface.");
        }
}

void DeviceManager::CreatePipelineObjects()
{
    m_GraphicsPipeline.m_SwapchainManager.CreateSwapchain(m_PhysicalDevice, m_LogicalDevice, m_Surface, m_WindowManager);
    m_GraphicsPipeline.m_SwapchainManager.CreateImageViews(m_LogicalDevice);
    m_GraphicsPipeline.CreateRenderPass(m_LogicalDevice, m_GraphicsPipeline.m_SwapchainManager.m_SwapchainImageFormat);
    m_GraphicsPipeline.CreatePipeline(m_LogicalDevice, m_GraphicsPipeline.m_SwapchainManager.m_SwapchainExtent);
    m_GraphicsPipeline.CreateFramebuffers(m_LogicalDevice);
    m_GraphicsPipeline.CreateCommandPool(m_PhysicalDevice, m_LogicalDevice, m_Surface);
    m_GraphicsPipeline.CreateCommandBuffers(m_LogicalDevice);
    m_GraphicsPipeline.CreateSyncObjects(m_LogicalDevice);
}

void DeviceManager::Destroy() const
{
    m_GraphicsPipeline.ResizeOrExitCleanup(m_LogicalDevice);

    m_LogicalDevice.destroyPipeline(m_GraphicsPipeline.m_Pipeline);
    m_LogicalDevice.destroyPipelineLayout(m_GraphicsPipeline.m_PipelineLayout);

    m_LogicalDevice.destroyRenderPass(m_GraphicsPipeline.m_RenderPass);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        m_LogicalDevice.destroySemaphore(m_GraphicsPipeline.m_ImageAvailableSemaphores[i]);
        m_LogicalDevice.destroySemaphore(m_GraphicsPipeline.m_RenderFinishedSemaphores[i]);
        m_LogicalDevice.destroyFence(m_GraphicsPipeline.m_InFlightFences[i]);
    }

    m_LogicalDevice.destroyCommandPool(m_GraphicsPipeline.m_CommandPool);

    m_LogicalDevice.destroy();
}

void DeviceManager::Draw()
{
    auto fnc = [this]() { m_GraphicsPipeline.DrawFrame(m_PhysicalDevice, m_LogicalDevice,
        m_Surface, m_WindowManager); };

    m_WindowManager.DoLoop(fnc);

    m_LogicalDevice.waitIdle();
}

bool DeviceManager::IsDeviceSuitable(const vk::PhysicalDevice& device) const
{
    const QueueFamilyIndices indices = Utilities::FindQueueFamilies(device, m_Surface);

    const bool extensionsSupported = m_GraphicsPipeline.m_SwapchainManager.CheckDeviceExtensionSupport(device);
    bool swapchainAdequate = false;

    if (extensionsSupported)
    {
        const auto swapchainSupport = SwapchainManager::QuerySwapchainSupport(device, m_Surface);
        swapchainAdequate = !swapchainSupport.m_Formats.empty() && !swapchainSupport.m_PresentModes.empty();
    }

    return indices.IsComplete() && swapchainAdequate;
}
