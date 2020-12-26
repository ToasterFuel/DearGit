#ifndef __STATUS_DATA_COLLECTIONS_H__
#define __STATUS_DATA_COLLECTIONS_H__

#include "../MultiSelectList.h"
#include "StatusData.h"
#include <vector>

class StatusDataCollection: public MultiSelectListItemProvider
{
public:
    std::vector<StatusData> files;
protected:
    virtual const char* ChildGetLabel(int index);
    virtual int ChildGetSize();
};

#endif