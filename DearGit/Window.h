#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "git/Repo.h"
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
    bool IsKeyPressed(int keyCode);
private:
    void DrawNavigation(int size);

    Repo* onlyRepo;
    LocalRegion localRegion;
    GLFWwindow *window;
};
#endif