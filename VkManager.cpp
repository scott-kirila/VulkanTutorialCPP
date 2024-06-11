//
// Created by hetan on 6/10/2024.
//
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include "VkManager.h"

#include <iostream>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VkManager::VkManager()
{
}

VkManager::~VkManager()
{

}

void VkManager::CreateInstance()
{
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    vk::ApplicationInfo applicationInfo( AppName.c_str(), 1, EngineName.c_str(), 1, VK_API_VERSION_1_1 );
    vk::InstanceCreateInfo instanceCreateInfo( {}, &applicationInfo );
    vk::Instance instance = vk::createInstance( instanceCreateInfo );
    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

    instance.destroy();
}

void VkManager::Run()
{
    CreateInstance();
    m_WindowManager.DoLoop();
}
