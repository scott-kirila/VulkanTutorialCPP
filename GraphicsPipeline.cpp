//
// Created by hetan on 6/15/2024.
//

#include "GraphicsPipeline.h"

void GraphicsPipeline::Create(vk::Device device, vk::Extent2D extent)
{
    auto vertexShaderCode = ShaderManager::ReadFile("../Shaders/vert.spv");
    auto fragmentShaderCode = ShaderManager::ReadFile("../Shaders/frag.spv");

    vk::ShaderModule vertexShaderModule = m_ShaderManager.CreateShaderModule(vertexShaderCode, device);
    vk::ShaderModule fragmentShaderModule = m_ShaderManager.CreateShaderModule(fragmentShaderCode, device);

    auto vertexShaderStageInfo = vk::PipelineShaderStageCreateInfo(
        {},
        vk::ShaderStageFlagBits::eVertex,
        vertexShaderModule,
        "main"
    );

    auto fragmentShaderStageInfo = vk::PipelineShaderStageCreateInfo(
        {},
        vk::ShaderStageFlagBits::eFragment,
        fragmentShaderModule,
        "main"
    );

    vk::PipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };

    auto vertexInputInfo = vk::PipelineVertexInputStateCreateInfo(
        {},
        0,
        nullptr,
        0,
        nullptr
    );

    auto inputAssembly = vk::PipelineInputAssemblyStateCreateInfo(
        {},
        vk::PrimitiveTopology::eTriangleList,
        vk::False
    );

    // Viewport settings
    auto viewport = vk::Viewport(
        0.0f,
        0.0f,
        static_cast<float>(extent.width),
        static_cast<float>(extent.height),
        0.0f,
        1.0f
    );

    auto scissor = vk::Rect2D(
        {0, 0},
        extent
    );

    std::vector<vk::DynamicState> dynamicStates = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor
    };

    auto dynamicState = vk::PipelineDynamicStateCreateInfo(
        {},
        dynamicStates
    );

    auto viewportState = vk::PipelineViewportStateCreateInfo(
        {},
        viewport,
        scissor
    );

    auto rasterizer = vk::PipelineRasterizationStateCreateInfo(
        {},
        vk::False,
        vk::False,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eClockwise,
        vk::False,
        0.0f,
        0.0f,
        0.0f,
        1.0f
    );

    auto multisampling = vk::PipelineMultisampleStateCreateInfo(
        {},
        vk::SampleCountFlagBits::e1,
        vk::False,
        1.0f,
        nullptr,
        vk::False,
        vk::False
    );

    auto colorBlendAttachment = vk::PipelineColorBlendAttachmentState(
        vk::False,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR
        | vk::ColorComponentFlagBits::eG
        | vk::ColorComponentFlagBits::eB
        | vk::ColorComponentFlagBits::eA
    );

    auto colorBlending = vk::PipelineColorBlendStateCreateInfo(
        {},
        vk::False,
        vk::LogicOp::eCopy,
        colorBlendAttachment,
        {0.0f, 0.0f, 0.0f, 0.0f}
    );

    auto pipelineLayoutInfo = vk::PipelineLayoutCreateInfo(
        {},
        0,
        nullptr,
        0,
        nullptr
    );

    m_PipelineLayout = device.createPipelineLayout(pipelineLayoutInfo);

    auto pipelineInfo = vk::GraphicsPipelineCreateInfo(
        {},
        shaderStages,
        &vertexInputInfo,
        &inputAssembly,
        {},
        &viewportState,
        &rasterizer,
        &multisampling,
        nullptr,
        &colorBlending,
        &dynamicState,
        m_PipelineLayout,
        m_RenderPass,
        0,
        VK_NULL_HANDLE,
        -1
    );

    auto result = device.createGraphicsPipeline(VK_NULL_HANDLE, pipelineInfo, nullptr);
    if (result.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to create graphics pipeline.");
    }
    m_Pipeline = result.value;

    device.destroyShaderModule(vertexShaderModule);
    device.destroyShaderModule(fragmentShaderModule);
}

void GraphicsPipeline::CreateRenderPass(vk::Device device, vk::Format format)
{
    auto colorAttachment = vk::AttachmentDescription(
        {},
        format,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR
    );

    auto colorAttachmentRef = vk::AttachmentReference(
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    );

    auto subpass = vk::SubpassDescription(
        {},
        vk::PipelineBindPoint::eGraphics,
        {},
        colorAttachmentRef
    );

    auto renderPassInfo = vk::RenderPassCreateInfo(
        {},
        colorAttachment,
        subpass
    );

    m_RenderPass = device.createRenderPass(renderPassInfo);
}
