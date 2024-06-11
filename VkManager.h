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

    static std::vector<const char *> GetRequiredExtensions();

private:
    void CreateInstance();

    std::string AppName = "Vulkan";
    std::string EngineName = "Vulkan.hpp";

    WindowManager m_WindowManager;
    #ifndef NDEBUG
    ValidationLayersManager m_ValidationManager;
    #endif
    vk::Instance m_Instance;
};



#endif //VKMANAGER_H
