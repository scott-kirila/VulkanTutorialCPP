//
// Created by Scott Kirila on 6/15/2024.
//

#include "GraphicsPipeline.h"

#include <iostream>

#include "Utilities.h"

void GraphicsPipeline::CreatePipeline(vk::Device device, vk::Extent2D extent)
{
    auto vertexShaderCode = ShaderManager::ReadFile("../Shaders/vert.spv");
    auto fragmentShaderCode = ShaderManager::ReadFile("../Shaders/frag.spv");

    vk::ShaderModule vertexShaderModule = ShaderManager::CreateShaderModule(vertexShaderCode, device);
    vk::ShaderModule fragmentShaderModule = ShaderManager::CreateShaderModule(fragmentShaderCode, device);

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

    auto bindingDescription = Vertex::GetBindingDescription();
    auto attributeDescriptions = Vertex::GetAttributeDescriptions();

    auto vertexInputInfo = vk::PipelineVertexInputStateCreateInfo(
        {},
        1,
        &bindingDescription,
        1,
        attributeDescriptions.data()
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

    auto dependency = vk::SubpassDependency(
        vk::SubpassExternal,
        0,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        {},
        vk::AccessFlagBits::eColorAttachmentWrite
    );

    auto renderPassInfo = vk::RenderPassCreateInfo(
        {},
        colorAttachment,
        subpass,
        dependency
    );

    m_RenderPass = device.createRenderPass(renderPassInfo);
}

void GraphicsPipeline::CreateFramebuffers(vk::Device device)
{
    m_Framebuffers.resize(m_SwapchainManager.m_SwapchainImageViews.size());

    for (size_t i = 0; i < m_SwapchainManager.m_SwapchainImageViews.size(); i++)
    {
        auto framebufferInfo = vk::FramebufferCreateInfo(
            {},
            m_RenderPass,
            m_SwapchainManager.m_SwapchainImageViews[i],
            m_SwapchainManager.m_SwapchainExtent.width,
            m_SwapchainManager.m_SwapchainExtent.height,
            1
        );

        m_Framebuffers[i] = device.createFramebuffer(framebufferInfo, nullptr);
    }
}

void GraphicsPipeline::CreateCommandPool(const vk::PhysicalDevice& physicalDevice,
                                         const vk::Device& logicalDevice, const vk::SurfaceKHR& surface)
{
    const auto queueFamilyIndices = Utilities::FindQueueFamilies(physicalDevice, surface);

    const auto poolInfo = vk::CommandPoolCreateInfo(
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        queueFamilyIndices.m_GraphicsFamily.value()
    );

    m_CommandPool = logicalDevice.createCommandPool(poolInfo, nullptr);
}

void GraphicsPipeline::CreateCommandBuffers(const vk::Device &device)
{
    m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    auto allocInfo = vk::CommandBufferAllocateInfo(
        m_CommandPool,
        vk::CommandBufferLevel::ePrimary,
        m_CommandBuffers.size()
    );

    m_CommandBuffers = device.allocateCommandBuffers(allocInfo);

}

void GraphicsPipeline::RecordCommandBuffer(const vk::CommandBuffer& commandBuffer, uint32_t imageIndex)
{
    auto beginInfo = vk::CommandBufferBeginInfo(
        {},
        nullptr
    );

    commandBuffer.begin(beginInfo);

    vk::ClearValue clearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};

    auto renderPassInfo = vk::RenderPassBeginInfo(
        m_RenderPass,
        m_Framebuffers[imageIndex],
        { {0, 0}, m_SwapchainManager.m_SwapchainExtent },
        clearColor
    );

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline);

    auto viewport = vk::Viewport(
        0.0f,
        0.0f,
        static_cast<float>(m_SwapchainManager.m_SwapchainExtent.width),
        static_cast<float>(m_SwapchainManager.m_SwapchainExtent.height),
        0.0f,
        1.0f
    );

    commandBuffer.setViewport(0, viewport);

    auto scissor = vk::Rect2D(
        {0, 0},
        m_SwapchainManager.m_SwapchainExtent
    );

    commandBuffer.setScissor(0, scissor);

    commandBuffer.draw(3, 1, 0, 0);

    commandBuffer.endRenderPass();

    commandBuffer.end();
}

void GraphicsPipeline::CreateSyncObjects(const vk::Device &device)
{
    m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    auto semaphoreInfo = vk::SemaphoreCreateInfo();
    auto fenceInfo = vk::FenceCreateInfo(
        vk::FenceCreateFlagBits::eSignaled
    );

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        m_ImageAvailableSemaphores[i] = device.createSemaphore(semaphoreInfo);
        m_RenderFinishedSemaphores[i] = device.createSemaphore(semaphoreInfo);
        m_InFlightFences[i] = device.createFence(fenceInfo);
    }
}

void GraphicsPipeline::DrawFrame(const vk::PhysicalDevice& physicalDevice, const vk::Device &logicalDevice,
    const vk::SurfaceKHR& surface, const WindowManager& windowManager)
{
    if (logicalDevice.waitForFences(m_InFlightFences[m_CurrentFrame], vk::True, UINT64_MAX) == vk::Result::eTimeout)
    {
        throw std::runtime_error("Timed out while waiting for fence.");
    }


    uint32_t imageIndex;
    vk::Result result = logicalDevice.acquireNextImageKHR(m_SwapchainManager.m_Swapchain, UINT64_MAX,
        m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR)
    {
        ResizeUpdate(physicalDevice, logicalDevice, surface, windowManager);
        return;
    }
    if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
    {
        throw std::runtime_error("Failed to acquire swapchain image.");
    }

    logicalDevice.resetFences(m_InFlightFences[m_CurrentFrame]);

    m_CommandBuffers[m_CurrentFrame].reset();
    RecordCommandBuffer(m_CommandBuffers[m_CurrentFrame], imageIndex);

    vk::Semaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlags(vk::PipelineStageFlagBits::eColorAttachmentOutput) };
    vk::Semaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };

    auto submitInfo = vk::SubmitInfo(
        waitSemaphores,
        waitStages,
        m_CommandBuffers[m_CurrentFrame],
        signalSemaphores
    );

    m_GraphicsQueue.submit(submitInfo, m_InFlightFences[m_CurrentFrame]);

    vk::SwapchainKHR swapchains[] = { m_SwapchainManager.m_Swapchain };

    auto presentInfo = vk::PresentInfoKHR(
        signalSemaphores,
        swapchains,
        imageIndex
    );

    try
    {
        result = m_PresentQueue.presentKHR(presentInfo);

        if (result == vk::Result::eSuboptimalKHR || WindowManager::m_FramebufferResized)
        {
            WindowManager::m_FramebufferResized = false;
            ResizeUpdate(physicalDevice, logicalDevice, surface, windowManager);
        } else if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("Failed to present swapchain image.");
        }
    } catch (vk::OutOfDateKHRError&)
    {
        WindowManager::m_FramebufferResized = false;
        ResizeUpdate(physicalDevice, logicalDevice, surface, windowManager);
    }

    m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void GraphicsPipeline::ResizeOrExitCleanup(const vk::Device& logicalDevice) const
{
    logicalDevice.waitIdle();

    for (const auto framebuffer : m_Framebuffers)
    {
        logicalDevice.destroyFramebuffer(framebuffer);
    }

    m_SwapchainManager.CleanupSwapchain(logicalDevice);

}

void GraphicsPipeline::ResizeUpdate(const vk::PhysicalDevice& physicalDevice, const vk::Device& logicalDevice,
    const vk::SurfaceKHR& surface, const WindowManager& windowManager)
{
    int width {}, height {};
    windowManager.GetFramebufferSize(width, height);

    do
    {
        windowManager.GetFramebufferSize(width, height);
        WindowManager::AwaitEvents();
    }
    while (width == 0 || height == 0);

    ResizeOrExitCleanup(logicalDevice);
    m_SwapchainManager.CreateSwapchain(physicalDevice, logicalDevice, surface, windowManager);
    m_SwapchainManager.CreateImageViews(logicalDevice);
    CreateFramebuffers(logicalDevice);
}
