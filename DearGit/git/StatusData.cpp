#include "StatusData.h"

StatusData::StatusData(const git_status_entry* entry) : entry(entry)
{

}

const char* StatusData::Path()
{
    return "";
}

GitStatus StatusData::GetStatus()
{
    return STATUS_UP_TO_DATE;
}

bool StatusData::IsStaged()
{
    return false;
}
