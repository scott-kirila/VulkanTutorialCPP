//
// Created by Scott Kirila on 6/15/2024.
//

#include "Utilities.h"

#include <vector>


vk::VertexInputBindingDescription Vertex::GetBindingDescription()
{
    return vk::VertexInputBindingDescription{
        0,
        sizeof(Vertex),
        vk::VertexInputRate::eVertex
    };
}

std::array<vk::VertexInputAttributeDescription, 2> Vertex::GetAttributeDescriptions()
{
    std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}


QueueFamilyIndices Utilities::FindQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
{
    QueueFamilyIndices indices;

    const std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.m_GraphicsFamily = i;
        }

        if (device.getSurfaceSupportKHR(i, surface))
        {
            indices.m_PresentFamily = i;
        }

        if (indices.IsComplete()) break;

        i++;
    }

    return indices;
}
