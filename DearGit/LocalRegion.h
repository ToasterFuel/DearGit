#ifndef __LOCAL_REGION_H__
#define __LOCAL_REGION_H__
#include "git/StatusDataCollection.h"
#include "MultiSelectList.h"
#include "Region.h"
#include "imgui/imgui.h"
#include "TextureHolder.h"
#include <set>

class LocalRegion: public Region
{
public:
    LocalRegion();
protected:
    virtual void DrawChildRegion(int width, int height);
	virtual bool ChildInit();
    virtual void ChildRefresh();
private:
    void AddToStaged();
    bool IndexHighlighted(int index);
    ImTextureID GetTextureId(TextureName textureName);
    bool DrawLabel(const char* name);

    MultiSelectList unstagedMultiSelectList;
    MultiSelectList stagedMultiSelectList;
    StatusDataCollection* unstagedFiles;
    StatusDataCollection* stagedFiles;
    std::set<int> highlightedIndices;
    int fileListWidth;
    int fileListHeight;
};
#endif
