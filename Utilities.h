//
// Created by Scott Kirila on 6/15/2024.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include <array>
#include <cstdint>
#include <optional>

#include "glm/glm.hpp"
#include "vulkan/vulkan.hpp"

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static vk::VertexInputBindingDescription GetBindingDescription();

    static std::array<vk::VertexInputAttributeDescription, 2> GetAttributeDescriptions();
};

struct QueueFamilyIndices
{
    [[nodiscard]] bool IsComplete() const
    {
        return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
    }

    std::optional<uint32_t> m_GraphicsFamily;
    std::optional<uint32_t> m_PresentFamily;
};

struct Utilities {
    [[nodiscard]] static QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device,
                                                                         const vk::SurfaceKHR& surface);

};



#endif //UTILITIES_H
