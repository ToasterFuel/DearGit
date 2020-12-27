#ifndef __MULTI_SELECT_LIST_H__
#define __MULTI_SELECT_LIST_H__

#include <vector>
#include <set>

class Window;

class MultiSelectListItemProvider
{
public:
    const char* GetLabel(int index);
    int GetSize();
protected:
    virtual const char* ChildGetLabel(int index) = 0;
    virtual int ChildGetSize() = 0;
};

class MultiSelectList
{
public:
    MultiSelectList(Window* window = nullptr, MultiSelectListItemProvider* provider = nullptr);
    void SetData(MultiSelectListItemProvider* provider);
    bool Draw();
    void ClearSelections();
    bool HasSelections();
    std::set<int> GetSelectedItems();
private:
    int lastSelected;
    Window* window;
    std::set<int> highlightedIndices;
    MultiSelectListItemProvider* provider;

    bool IndexHighlighted(int index);
};
#endif