#include "StatusDataCollection.h"

const char* StatusDataCollection::ChildGetLabel(int index)
{
    return files[index].Path();
}

int StatusDataCollection::ChildGetSize()
{
    return files.size();
}
