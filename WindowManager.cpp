//
// Created by hetan on 6/10/2024.
//

#include "WindowManager.h"

WindowManager::WindowManager()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_Window = glfwCreateWindow(static_cast<int>(WIDTH), static_cast<int>(HEIGHT),
        "Vulkan", nullptr, nullptr);
}

WindowManager::~WindowManager()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void WindowManager::DoLoop()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();

        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_Window, true);
        }
    }
}
