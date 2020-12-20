#include "Window.h"
#include <iostream>
#include <git2.h>

int main()
{
    Window window;
    if(!window.Init())
        return -1;

    git_repository *repo = NULL;
    std::cout << git_libgit2_init() << "\n";
    std::cout << git_repository_init(&repo, "D:\\Workspace\\tmp\\", false) << "\n";
    if (repo == NULL)
        std::cout << "It still fucked!\n";

    while(window.ContinueRunning())
    {
        window.DrawFrame();
    }
    window.CleanUp();
    return 0;
}
