#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "LocalRegion.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    bool Init();
    bool ContinueRunning();
    void DrawFrame();
    void CleanUp();
private:
    void DrawNavigation(int size);
    LocalRegion localRegion;
    GLFWwindow *window;
};
#endif