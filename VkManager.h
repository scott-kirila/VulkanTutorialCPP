//
// Created by hetan on 6/10/2024.
//

#ifndef VKMANAGER_H
#define VKMANAGER_H

#include "vulkan/vulkan.hpp"

#include "WindowManager.h"

#ifdef NDEBUG
#include "ValidationLayersManager.h"
#endif

class VkManager {
public:
    VkManager();
    ~VkManager();

    void Run();

private:
    void CreateInstance();

    std::string AppName = "Vulkan";
    std::string EngineName = "No Engine";

#ifdef NDEBUG
    ValidationLayersManager m_ValidationLayersManager;
#endif

    WindowManager m_WindowManager;
    // vk::DynamicLoader m_DynamicLoader;
    vk::Instance m_Instance;
};



#endif //VKMANAGER_H
