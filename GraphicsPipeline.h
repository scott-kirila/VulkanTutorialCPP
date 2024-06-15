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
    void CreateRenderPass(vk::Device, vk::Format format);

    vk::RenderPass m_RenderPass;
    vk::PipelineLayout m_PipelineLayout;
    vk::Pipeline m_Pipeline;

private:
    ShaderManager m_ShaderManager;
};



#endif //GRAPHICSPIPELINE_H
