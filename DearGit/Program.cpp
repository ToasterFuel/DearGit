#include "Window.h"

int main()
{
    Window window;
    if(!window.Init())
        return -1;

    while(window.ContinueRunning())
    {
        window.DrawFrame();
    }
    window.CleanUp();
    return 0;
}
