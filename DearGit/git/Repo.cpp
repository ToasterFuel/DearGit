#include "Repo.h"

#include <iostream>

Repo::Repo(const char* baseDirectory): baseDirectory(baseDirectory), statusList(nullptr), gitRepo(nullptr), gitIndex(nullptr)
{
    singleFileArray = (char**)malloc(sizeof(char*));
    *singleFileArray = (char*)malloc(sizeof(char) * MAX_FILE_PATH_SIZE + 1);
    singleFileList.count = 1;
    singleFileList.strings = singleFileArray;
    CopyToSingleFile("");
    CopyToSingleFile("Modified.txt");
}

Repo::~Repo()
{
    StatusCleanUp();
    HeadCleanUp();
    free(*singleFileArray);
    free(singleFileArray);
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
    for(int i = 0; i < GetStatusItemCount(); i++)
    {
        const git_status_entry* entry = git_status_byindex(statusList, i);
        if((entry->status & flags) != 0)
        {
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

void Repo::HeadCleanUp()
{
    git_reference_free(headReference);
    git_object_free(headCommit);
    headReference = nullptr;
    headCommit = nullptr;
}

bool Repo::GetHead()
{
    if(headReference != nullptr || headCommit != nullptr)
        HeadCleanUp();
    if(git_repository_head(&headReference, gitRepo) != 0)
    {
        //TODO error handling
        std::cout << "Failure getting reference to head\n";
        return false;
    }

    if(git_reference_peel(&headCommit, headReference, GIT_OBJ_COMMIT) != 0)
    {
        //TODO error handling
        std::cout << "Failure getting commit to head\n";
        return false;
    }

    return true;
}

bool Repo::GetIndex()
{
    if(gitIndex != nullptr)
        git_index_free(gitIndex);
    if(git_repository_index(&gitIndex, gitRepo) != 0)
    {
        std::cout << "Error getting index! \n";
        //TODO do more with the error code
        return false;
    }
    return true;
}

bool Repo::SaveIndex()
{
    if(gitIndex == nullptr)
        return true;
    if(git_index_write(gitIndex) != 0)
    {
        std::cout << "Error writing index\n";
        //TODO do more with the error code
        return false;
    }
    return true;
}

bool Repo::UnstageFile(const char* path)
{
    if(headReference == nullptr || headCommit == nullptr)
        GetHead();
    if(!CopyToSingleFile(path))
        return false;

    if(git_reset_default(gitRepo, headCommit, &singleFileList) != 0)
    {
        std::cout << "Failed to unstage file: " << path << "\n";
        return false;
    }

    return true;
}

bool Repo::Commit(const char* message)
{
    git_oid commit_oid, tree_oid;
    git_signature *signature;
    git_tree *tree;
    git_object *parent = nullptr;
    git_reference *ref = nullptr;
    //TODO Error handle when the signature isn't set up
    git_signature_default(&signature, gitRepo);

    if(!GetIndex())
    {
        std::cout << "Failed to commit because of the index\n";
        return false;
    }

    int parentCount = 1;
    int error = git_revparse_ext(&parent, &ref, gitRepo, HEAD);
    if (error == GIT_ENOTFOUND) {

        printf("HEAD not found. Creating first commit\n");
        error = 0;
        parentCount = 0;
    }
    else if (error != 0) {
        const git_error *err = git_error_last();
        if (err) printf("ERROR %d: %s\n", err->klass, err->message);
        else printf("ERROR %d: no detailed info\n", error);
    }

    if(git_index_write_tree(&tree_oid, gitIndex) < 0)
    {
        //TODO Error handling
        std::cout << "Error writing tree before commiting\n";
        git_signature_free(signature);
        git_object_free(parent);
        return false;
    }
    if(git_tree_lookup(&tree, gitRepo, &tree_oid) < 0)
    {
        //TODO Error handling
        std::cout << "Error getting tree for commiting\n";
        git_signature_free(signature);
        git_object_free(parent);
        return false;
    }
    if(git_commit_create_v(&commit_oid, gitRepo, HEAD, signature, signature, NULL, message, tree, parentCount, parent) < 0)
    {
        //TODO Error handling
        std::cout << "Error commiting\n";
        git_signature_free(signature);
        git_object_free(parent);
        git_tree_free(tree);
        return false;
    }

    /*
        const git_error *err = git_error_last();
        if (err) printf("ERROR %d: %s\n", err->klass, err->message);
        else printf("ERROR %d: no detailed info\n", error);
    */

    return true;
}

bool Repo::CopyToSingleFile(const char* path)
{
    int length = strlen(path);
    if(length >= MAX_FILE_PATH_SIZE)
        return false;
    strcpy_s(*singleFileArray, _TRUNCATE, path);
    return true;
}

bool Repo::StageFile(const char* path)
{
    if(gitIndex == nullptr)
    {
        if(!GetIndex())
            return false;
    }

    if(git_index_add_bypath(gitIndex, path) != 0)
    {
        std::cout << "Error staging file: " << path << "\n";
        //TODO do more with the error code
        return false;
    }

    return true;

}

int Repo::GetStatusItemCount()
{
    return git_status_list_entrycount(statusList);
}
