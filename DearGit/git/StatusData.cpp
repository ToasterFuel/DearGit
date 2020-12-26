#include "StatusData.h"

StatusData::StatusData(const git_status_entry* entry) : entry(entry)
{

}

const char* StatusData::Path()
{
    if(IsStaged())
        return entry->head_to_index->new_file.path;
    return entry->index_to_workdir->new_file.path;
}

GitStatus StatusData::GetStatus()
{
    switch(entry->status)
    {
    case GIT_STATUS_CURRENT:
        return STATUS_UP_TO_DATE;
    case GIT_STATUS_WT_NEW:
        return STATUS_NEW;
    case GIT_STATUS_WT_MODIFIED:
        return STATUS_MODIFIED;
    case GIT_STATUS_WT_DELETED:
        return STATUS_DELETED;
    case GIT_STATUS_WT_TYPECHANGE:
        return STATUS_MOVED;
    case GIT_STATUS_WT_RENAMED:
        return STATUS_MOVED;
    case GIT_STATUS_WT_UNREADABLE:
        return STATUS_MOVED;
    case GIT_STATUS_IGNORED:
        return STATUS_FUTURE_WORK_IS_NEEDED;
    case GIT_STATUS_CONFLICTED:
        return STATUS_FUTURE_WORK_IS_NEEDED;
    case GIT_STATUS_INDEX_NEW:
        return STATUS_NEW;
    case GIT_STATUS_INDEX_MODIFIED:
        return STATUS_MODIFIED;
    case GIT_STATUS_INDEX_DELETED:
        return STATUS_DELETED;
    case GIT_STATUS_INDEX_RENAMED:
        return STATUS_MOVED;
    case GIT_STATUS_INDEX_TYPECHANGE:
        return STATUS_MOVED;
    }
    return STATUS_UP_TO_DATE;
}

bool StatusData::IsStaged()
{
    switch(entry->status)
    {
    case GIT_STATUS_CURRENT:
    case GIT_STATUS_WT_NEW:
    case GIT_STATUS_WT_MODIFIED:
    case GIT_STATUS_WT_DELETED:
    case GIT_STATUS_WT_TYPECHANGE:
    case GIT_STATUS_WT_RENAMED:
    case GIT_STATUS_WT_UNREADABLE:
    case GIT_STATUS_IGNORED:
    case GIT_STATUS_CONFLICTED:
        return false;
    case GIT_STATUS_INDEX_NEW:
    case GIT_STATUS_INDEX_MODIFIED:
    case GIT_STATUS_INDEX_DELETED:
    case GIT_STATUS_INDEX_RENAMED:
    case GIT_STATUS_INDEX_TYPECHANGE:
        return true;
    }
    return false;
}
