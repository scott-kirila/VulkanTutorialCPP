//
// Created by hetan on 6/10/2024.
//

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "GLFW/glfw3.h"

class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    void DoLoop();

    GLFWwindow* m_Window;

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};



#endif //WINDOWMANAGER_H
