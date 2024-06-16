//
// Created by Scott Kirila on 6/15/2024.
//

#ifndef GRAPHICSPIPELINE_H
#define GRAPHICSPIPELINE_H

#include "ShaderManager.h"
#include "SwapchainManager.h"

constexpr int MAX_FRAMES_IN_FLIGHT {2};

class GraphicsPipeline {
public:
    void CreatePipeline(vk::Device device, vk::Extent2D extent);
    void CreateRenderPass(vk::Device device, vk::Format format);
    void CreateFramebuffers(vk::Device device);
    void CreateCommandPool(const vk::PhysicalDevice& physicalDevice, const vk::Device& logicalDevice, const vk::SurfaceKHR& surface);
    void CreateCommandBuffers(const vk::Device& device);
    void RecordCommandBuffer(const vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
    void CreateSyncObjects(const vk::Device& device);
    void DrawFrame(const vk::PhysicalDevice& physicalDevice, const vk::Device& logicalDevice,
        const vk::SurfaceKHR& surface, const WindowManager& windowManager);
    void ResizeOrExitCleanup(const vk::Device& logicalDevice) const;
    void ResizeUpdate(const vk::PhysicalDevice& physicalDevice, const vk::Device& logicalDevice,
        const vk::SurfaceKHR& surface, const WindowManager& windowManager);

    SwapchainManager m_SwapchainManager;
    vk::RenderPass m_RenderPass;
    vk::PipelineLayout m_PipelineLayout;
    vk::Pipeline m_Pipeline;
    std::vector<vk::Framebuffer> m_Framebuffers;
    vk::CommandPool m_CommandPool;
    std::vector<vk::CommandBuffer> m_CommandBuffers;

    std::vector<vk::Semaphore> m_ImageAvailableSemaphores;
    std::vector<vk::Semaphore> m_RenderFinishedSemaphores;
    std::vector<vk::Fence> m_InFlightFences;

    bool m_FramebufferResized {false};

    vk::Queue m_GraphicsQueue;
    vk::Queue m_PresentQueue;

private:
    ShaderManager m_ShaderManager;
    uint32_t m_CurrentFrame {};
};

#endif //GRAPHICSPIPELINE_H
