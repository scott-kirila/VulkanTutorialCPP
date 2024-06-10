//
// Created by hetan on 6/10/2024.
//

#ifndef VKMANAGER_H
#define VKMANAGER_H

#include "WindowManager.h"
#include "vulkan/vulkan.hpp"

class VkManager {
public:
    VkManager();
    ~VkManager();

    void Run();

private:
    void CreateInstance();

    std::string AppName = "Vulkan";
    std::string EngineName = "No Engine";

    WindowManager m_WindowManager;
    vk::Instance m_Instance;
};



#endif //VKMANAGER_H
