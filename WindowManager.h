//
// Created by Scott Kirila on 6/10/2024.
//

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    void GetFramebufferSize(int& width, int& height) const;

    void DoLoop() const;

    GLFWwindow* m_Window;

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};



#endif //WINDOWMANAGER_H
