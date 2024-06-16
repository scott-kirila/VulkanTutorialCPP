//
// Created by Scott Kirila on 6/10/2024.
//

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#define GLFW_INCLUDE_VULKAN
#include <functional>

#include "GLFW/glfw3.h"

class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    void GetFramebufferSize(int& width, int& height) const;

    void DoLoop(const std::function<void(void)> &fcn) const;

    GLFWwindow* m_Window;

private:
    const uint32_t WIDTH = 1920;
    const uint32_t HEIGHT = 1080;
};



#endif //WINDOWMANAGER_H
