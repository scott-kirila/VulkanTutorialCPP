//
// Created by Scott Kirila on 6/10/2024.
//

#include "WindowManager.h"

WindowManager::WindowManager()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_Window = glfwCreateWindow(static_cast<int>(WIDTH), static_cast<int>(HEIGHT),
        "Vulkan Tutorial", nullptr, nullptr);
    glfwSetFramebufferSizeCallback(m_Window, FramebufferResizeCallback);
}

WindowManager::~WindowManager()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void WindowManager::GetFramebufferSize(int &width, int &height) const
{
    glfwGetFramebufferSize(m_Window, &width, &height);
}

void WindowManager::DoLoop(const std::function<void()> &fcn) const
{
    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();

        fcn();

        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_Window, true);
        }
    }
}

void WindowManager::AwaitEvents()
{
    glfwWaitEvents();
}

void WindowManager::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    m_FramebufferResized = true;
}
