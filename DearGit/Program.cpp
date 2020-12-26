#include "Window.h"
#include "git/Repo.h"
#include <iostream>
#include <git2.h>

int main()
{
    Window window;
    if(!window.Init())
        return -1;

    ////git_repository *repo = NULL;
    ////std::cout << git_libgit2_init() << "\n";




    ////Repo myRepo("D:\\Workspace\\tmp\\");
    ////std::cout << "init repo: " << myRepo.Init() << "\n";
    ////std::cout << "status: " << myRepo.GitStatus() << "\n";
    ////std::cout << "count: " << myRepo.GetStatusItemCount() << "\n";
    ////std::vector<StatusData> shit;
    ////std::vector<StatusData> peepee;
    ////myRepo.GetUnstagedFiles(shit);
    ////myRepo.GetStagedFiles(peepee);
    ////std::cout << "unstaged size: " << shit.size() << " staged size: " << peepee.size() << "\n";












    //Or call git_repository_open()
    ////std::cout << git_repository_init(&repo, "D:\\Workspace\\tmp\\", false) << "\n";
    ////if (repo == NULL)
        ////std::cout << "It still fucked!\n";

    ////git_status_list* statusList;
    ////std::cout << "status: " << git_status_list_new(&statusList, repo, NULL) << "\n";
    ////std::cout << "status count: " << git_status_list_entrycount(statusList) << "\n";
    ////for(int i = 0; i < git_status_list_entrycount(statusList); i++)
    ////{
        ////const git_status_entry* entry = git_status_byindex(statusList, i);
        ////std::cout << "status enum: " << entry->status << "\n";
        ////std::cout << "index filesChanged: " << entry->index_to_workdir->nfiles << " status: " << entry->index_to_workdir->status << " new file path: " << entry->index_to_workdir->new_file.path << "\n";
        //std::cout << "index filesChanged: " << entry->head_to_index->nfiles << "\n";
    ////}

    ////git_index* gitIndex;
    ////std::cout << "\nGetting index: " << git_repository_index(&gitIndex, repo) << "\n";
    ////std::cout << "index count: " << git_index_entrycount(gitIndex) << "\n";
    /*
    for(int i = 0; i < git_index_entrycount(gitIndex); i++)
    {
        const git_index_entry* entry = git_index_get_byindex(gitIndex, i);
        std::cout << "path? " << entry->path << " adding this bitch? " << git_index_add(gitIndex, entry) <<  " staging this bitch: " << git_index_entry_stage(entry) << "\n";
    }
    */
    ////std::cout << "add by path: " << git_index_add_bypath(gitIndex, "modified.txt") << "\n";
    //size_t position = -1;
    //std::cout << "finding modified.txt: " << git_index_find(&position, gitIndex, "modified.txt") << " position: " << position << "\n";
    //const git_index_entry* entry = git_index_get_byindex(gitIndex, position);
    //std::cout << "Staging? " << git_index_entry_stage(entry) << "\n";
    ////git_index_write(gitIndex);

    ////git_status_list_free(statusList);
    ////git_index_free(gitIndex);

    while(window.ContinueRunning())
    {
        window.DrawFrame();
    }
    window.CleanUp();
    return 0;
}
