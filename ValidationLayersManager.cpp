//
// Created by hetan on 6/10/2024.
//

#include "ValidationLayersManager.h"

#include <vulkan/vulkan.hpp>

bool ValidationLayersManager::CheckValidationLayerSupport() const
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    for (const auto &layerName: validationLayers)
    {
        auto isAvailable = [layerName](vk::LayerProperties layerProperties)
        {
            return strcmp(layerName, layerProperties.layerName) == 0;
        };
        if (auto it = std::ranges::find_if(
                availableLayers, isAvailable);
            it == availableLayers.end())
            return false;
    }

    return true;
}
