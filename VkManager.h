//
// Created by Scott Kirila on 6/10/2024.
//

#ifndef VKMANAGER_H
#define VKMANAGER_H

#include "DeviceManager.h"
#include "ValidationLayersManager.h"
#include "vulkan/vulkan.hpp"

class VkManager {
public:
    VkManager();
    ~VkManager();

    void Run();

    static std::vector<const char *> GetRequiredExtensions();

private:
    void CreateInstance();

    std::string AppName = "Vulkan";
    std::string EngineName = "Vulkan.hpp";

    ValidationLayersManager m_ValidationManager;
    vk::Instance m_Instance;
    DeviceManager m_DeviceManager;
};



#endif //VKMANAGER_H
