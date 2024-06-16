//
// Created by Scott Kirila on 6/15/2024.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstdint>
#include <optional>

#include "vulkan/vulkan.hpp"

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
