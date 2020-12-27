#ifndef __REPO_H__
#define __REPO_H__

#include "StatusData.h"
#include <vector>
#include <git2.h>

#define MAX_FILE_PATH_SIZE 4096

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
    bool SaveIndex();
    bool GetIndex();
    bool StageFile(const char* path);
    bool UnstageFile(const char* path);

    virtual ~Repo();

private:
    char** singleFileArray;
    git_strarray singleFileList;
    const char* baseDirectory;
    git_index* gitIndex;
    git_status_list* statusList;
    git_repository* gitRepo;
    git_reference *headReference;
    git_object *headCommit;

    bool GetHead();
    void HeadCleanUp();
    bool CopyToSingleFile(const char* path);
    void AddFiles(std::vector<StatusData>& files, int flags);
};

#endif
