//
// Created by hetan on 6/10/2024.
//

#ifndef VKMANAGER_H
#define VKMANAGER_H

#include "ValidationLayersManager.h"
#include "vulkan/vulkan.hpp"
#include "WindowManager.h"

class VkManager {
public:
    VkManager();
    ~VkManager();

    void Run();

    std::vector<const char *> GetRequiredExtensions();

private:
    void CreateInstance();

    std::string AppName = "Vulkan";
    std::string EngineName = "Vulkan.hpp";

    WindowManager m_WindowManager;
    vk::DebugUtilsMessengerEXT m_DebugMessenger;
    ValidationLayersManager m_ValidationManager;
    vk::Instance m_Instance;
};



#endif //VKMANAGER_H
