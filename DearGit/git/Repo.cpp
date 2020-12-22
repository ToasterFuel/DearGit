#include "Repo.h"

#include <iostream>

Repo::Repo(const char* baseDirectory): baseDirectory(baseDirectory), statusList(nullptr), gitRepo(nullptr)
{
}

void Repo::GetUnstagedFiles(std::vector<StatusData>& files)
{
    if(statusList == nullptr)
    {
        std::cout << "Trying to get status information without calling status()\n";
        return;
    }
    files.clear();
    AddFiles(files, GIT_STATUS_WT_NEW | 
            GIT_STATUS_WT_MODIFIED |
            GIT_STATUS_WT_DELETED |
            GIT_STATUS_WT_TYPECHANGE |
            GIT_STATUS_WT_RENAMED | 
            GIT_STATUS_WT_UNREADABLE);
}

void Repo::GetStagedFiles(std::vector<StatusData>& files)
{
    if(statusList == nullptr)
    {
        std::cout << "Trying to get status information without calling status()\n";
        return;
    }
    files.clear();
    AddFiles(files, GIT_STATUS_INDEX_NEW |
            GIT_STATUS_INDEX_MODIFIED |
            GIT_STATUS_INDEX_DELETED |
            GIT_STATUS_INDEX_RENAMED);
}

void Repo::AddFiles(std::vector<StatusData>& files, int flags)
{
    std::cout << "flag: " << flags << "\n";
    for(int i = 0; i < GetStatusItemCount(); i++)
    {
        const git_status_entry* entry = git_status_byindex(statusList, i);
        std::cout << "\tstatus: " << entry->status << " yeah math! " << (entry->status & flags) << "\n";
        if((entry->status & flags) != 0)
        {
            std::cout << "HOOO WE ARE ADDING TO THE LIST!\n";
            files.push_back(StatusData(entry));
        }
    }
}

bool Repo::Init()
{
    if(git_repository_open(&gitRepo, baseDirectory) != 0)
    {
        std::cout << "Error opening local repository: " << baseDirectory << "\n";
        //TODO, what are the error codes? Look up how to do that!
        return false;
    }

    return true;
}

void Repo::StatusCleanUp()
{
    if(statusList == nullptr)
        return;
    git_status_list_free(statusList);
    statusList = nullptr;
}

bool Repo::GitStatus()
{
    StatusCleanUp();
    int status = git_status_list_new(&statusList, gitRepo, nullptr);
    if(status != 0)
    {
        std::cout << "Error getting status of local repository: " << baseDirectory << "\n";
        return false;
    }

    return true;
}

int Repo::GetStatusItemCount()
{
    return git_status_list_entrycount(statusList);
}
