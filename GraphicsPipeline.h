//
// Created by hetan on 6/15/2024.
//

#ifndef GRAPHICSPIPELINE_H
#define GRAPHICSPIPELINE_H

#include "ShaderManager.h"
#include "SwapchainManager.h"

class GraphicsPipeline {
public:
    void CreatePipeline(vk::Device device, vk::Extent2D extent);
    void CreateRenderPass(vk::Device device, vk::Format format);
    void CreateFramebuffers(vk::Device device);
    void CreateCommandPool(const vk::PhysicalDevice& physicalDevice, const vk::Device& logicalDevice, const vk::SurfaceKHR& surface);
    void CreateCommandBuffer(const vk::Device& device);
    void RecordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
    void CreateSyncObjects(const vk::Device& device);
    void DrawFrame(const vk::Device& device);

    SwapchainManager m_SwapchainManager;
    vk::RenderPass m_RenderPass;
    vk::PipelineLayout m_PipelineLayout;
    vk::Pipeline m_Pipeline;
    std::vector<vk::Framebuffer> m_Framebuffers;
    vk::CommandPool m_CommandPool;
    vk::CommandBuffer m_CommandBuffer;

    vk::Semaphore m_ImageAvailable;
    vk::Semaphore m_RenderFinished;
    vk::Fence m_InFlight;

    vk::Queue m_GraphicsQueue;
    vk::Queue m_PresentQueue;

private:
    ShaderManager m_ShaderManager;
};



#endif //GRAPHICSPIPELINE_H
