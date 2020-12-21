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
    //Or call git_repository_open()
    std::cout << git_repository_init(&repo, "D:\\Workspace\\tmp\\", false) << "\n";
    if (repo == NULL)
        std::cout << "It still fucked!\n";

    git_status_list* statusList;
    std::cout << "status: " << git_status_list_new(&statusList, repo, NULL) << "\n";
    std::cout << "status count: " << git_status_list_entrycount(statusList) << "\n";
    for(int i = 0; i < git_status_list_entrycount(statusList); i++)
    {
        const git_status_entry* entry = git_status_byindex(statusList, i);
        std::cout << "status enum: " << entry->status << "\n";
        std::cout << "index filesChanged: " << entry->index_to_workdir->nfiles << " status: " << entry->index_to_workdir->status << " new file path: " << entry->index_to_workdir->new_file.path << "\n";
    }
    git_status_list_free(statusList);

    while(window.ContinueRunning())
    {
        window.DrawFrame();
    }
    window.CleanUp();
    return 0;
}
