#include "Repo.h"

#include <sstream>
#include <iostream>

Repo::Repo(const char* baseDirectory): baseDirectory(baseDirectory), statusList(nullptr), gitRepo(nullptr), gitIndex(nullptr)
{
    singleFileArray = (char**)malloc(sizeof(char*));
    *singleFileArray = (char*)malloc(sizeof(char) * MAX_FILE_PATH_SIZE + 1);
    singleFileList.count = 1;
    singleFileList.strings = singleFileArray;
    CopyToSingleFile("");
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
    std::ostringstream msg("Error opening local repository: ");
    msg << baseDirectory;
    if(DidFailAndPrintError(git_repository_open(&gitRepo, baseDirectory), msg.str().c_str()))
        return false;

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

    std::ostringstream msg("Error getting status of local repository: ");
    msg << baseDirectory;
    if(DidFailAndPrintError(git_status_list_new(&statusList, gitRepo, nullptr), msg.str().c_str()))
        return false;

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

    if(DidFailAndPrintError(git_repository_head(&headReference, gitRepo), "Failure getting reference to head"))
        return false;

    if(DidFailAndPrintError(git_reference_peel(&headCommit, headReference, GIT_OBJ_COMMIT), "Failure getting commit to head"))
        return false;

    return true;
}

bool Repo::GetIndex()
{
    if(gitIndex != nullptr)
        git_index_free(gitIndex);
    if(DidFailAndPrintError(git_repository_index(&gitIndex, gitRepo), "Error getting index!"))
        return false;
    return true;
}

bool Repo::SaveIndex()
{
    if(gitIndex == nullptr)
        return true;
    if(DidFailAndPrintError(git_index_write(gitIndex), "Error writing index"))
        return false;
    return true;
}

bool Repo::UnstageFile(const char* path)
{
    if(headReference == nullptr || headCommit == nullptr)
        GetHead();
    if(!CopyToSingleFile(path))
        return false;

    std::ostringstream msg("Failed to unstage file: ");
    msg << path;
    if(DidFailAndPrintError(git_reset_default(gitRepo, headCommit, &singleFileList), msg.str().c_str()))
        return false;

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
    if(error == GIT_ENOTFOUND)
    {
        error = 0;
        parentCount = 0;
    }
    else if(error != 0)
    {
        DidFailAndPrintError(error, "Error finding the parent (last commit?) before committing");
        return false;
    }

    if(DidFailAndPrintError(git_index_write_tree(&tree_oid, gitIndex), "Error writing tree before committing"))
    {
        git_signature_free(signature);
        git_object_free(parent);
        return false;
    }
    if(DidFailAndPrintError(git_tree_lookup(&tree, gitRepo, &tree_oid), "Error getting tree for committing"))
    {
        git_signature_free(signature);
        git_object_free(parent);
        return false;
    }
    if(DidFailAndPrintError(git_commit_create_v(&commit_oid, gitRepo, HEAD, signature, signature, NULL, message, tree, parentCount, parent), "Error committing"))
    {
        git_signature_free(signature);
        git_object_free(parent);
        git_tree_free(tree);
        return false;
    }

    git_signature_free(signature);
    git_object_free(parent);
    git_tree_free(tree);

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

bool Repo::DidFailAndPrintError(int errorCode, const char* appErrorMessage)
{
    if(errorCode >= 0)
        return false;

    const git_error *err = git_error_last();
    if(err)
        std::cout << appErrorMessage << " ErrorCode: " << errorCode << " errorClass: " << err->klass << " error message: " << err->message << "\n";
    else
        std::cout << appErrorMessage << " ErrorCode: " << errorCode << " (no additional information for libgit)\n";
    return true;
}

bool Repo::StageFile(const char* path)
{
    if(gitIndex == nullptr)
    {
        if(!GetIndex())
            return false;
    }

    std::ostringstream msg("Error staging file: ");
    msg << path;

    if(DidFailAndPrintError(git_index_add_bypath(gitIndex, path), msg.str().c_str()))
        return false;

    return true;

}

int Repo::GetStatusItemCount()
{
    return git_status_list_entrycount(statusList);
}
