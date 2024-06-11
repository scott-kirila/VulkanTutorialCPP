//
// Created by hetan on 6/10/2024.
//

#ifndef VKMANAGER_H
#define VKMANAGER_H

#include "vulkan/vulkan.hpp"
#include "WindowManager.h"

inline vk::DispatchLoaderDynamic dldi;

class VkManager {
public:
    VkManager();
    ~VkManager();

    void Run();

private:
    void CreateInstance();

    std::string AppName = "Vulkan";
    std::string EngineName = "Vulkan.hpp";

    WindowManager m_WindowManager{};
    vk::DynamicLoader m_DynamicLoader{};
    vk::Instance m_Instance{};
};



#endif //VKMANAGER_H
