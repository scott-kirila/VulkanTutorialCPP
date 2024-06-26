cmake_minimum_required(VERSION 3.28)
project(VulkanTutorialCPP)

set(CMAKE_CXX_STANDARD 23)

add_executable(VulkanTutorialCPP main.cpp
        WindowManager.cpp
        WindowManager.h
        VkManager.cpp
        VkManager.h
        ValidationLayersManager.cpp
        ValidationLayersManager.h
        DeviceManager.cpp
        DeviceManager.h
        SwapchainManager.cpp
        SwapchainManager.h
        ShaderManager.cpp
        ShaderManager.h
        GraphicsPipeline.cpp
        GraphicsPipeline.h
        Utilities.cpp
        Utilities.h)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

target_link_libraries(VulkanTutorialCPP PRIVATE glfw glm::glm Vulkan::Vulkan)