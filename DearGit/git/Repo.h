#ifndef __REPO_H__
#define __REPO_H__

#include "StatusData.h"
#include <vector>
#include <git2.h>

class Repo
{
public:
    Repo(const char* baseDirectory = "");
    bool Init();
    /*
     * Basically the same as running 'git status' in cmd. Returns false if error happened.
     */
    bool GitStatus();
    void StatusCleanUp();

    /*
     * Total number of items found with the last GitStatus call.
     */
    int GetStatusItemCount();
    void GetUnstagedFiles(std::vector<StatusData>& files);
    void GetStagedFiles(std::vector<StatusData>& files);

private:
    const char* baseDirectory;
    git_status_list* statusList;
    git_repository* gitRepo;
    void AddFiles(std::vector<StatusData>& files, int flags);
};

#endif
