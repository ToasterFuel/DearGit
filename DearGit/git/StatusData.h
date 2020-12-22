#ifndef __STATUS_DATA_H__
#define __STATUS_DATA_H__

#include <git2.h>

enum GitStatus
{
    STATUS_UP_TO_DATE,
    STATUS_NEW,
    STATUS_MODIFIED,
    STATUS_DELETED,
    STATUS_MOVED,
};

class StatusData
{
public:
    StatusData(const git_status_entry* entry);
    const char* Path();
    GitStatus GetStatus();
    bool IsStaged();
private:
    const git_status_entry* entry;
};

#endif
