//
// Created by hetan on 6/15/2024.
//

#ifndef GRAPHICSPIPELINE_H
#define GRAPHICSPIPELINE_H

#include "ShaderManager.h"
#include "SwapchainManager.h"

class GraphicsPipeline {
public:
    void Create(vk::Device device, vk::Extent2D extent);
    void CreateRenderPass(vk::Device device, vk::Format format);
    void CreateFramebuffers(vk::Device device);

    SwapchainManager m_SwapchainManager;
    vk::RenderPass m_RenderPass;
    vk::PipelineLayout m_PipelineLayout;
    vk::Pipeline m_Pipeline;
    std::vector<vk::Framebuffer> m_Framebuffers;

private:
    ShaderManager m_ShaderManager;
};



#endif //GRAPHICSPIPELINE_H
