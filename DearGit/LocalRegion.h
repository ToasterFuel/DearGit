#ifndef __LOCAL_REGION_H__
#define __LOCAL_REGION_H__
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
private:
    bool IndexHighlighted(int index);
    ImTextureID GetTextureId(TextureName textureName);
    bool DrawLabel(const char* name);

    std::set<int> highlightedIndices;
    int fileListWidth;
    int fileListHeight;
};
#endif
