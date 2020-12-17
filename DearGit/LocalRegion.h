#ifndef __LOCAL_REGION_H__
#define __LOCAL_REGION_H__
#include "Region.h"
#include "imgui/imgui.h"
#include "TextureHolder.h"

class LocalRegion: public Region
{
public:
    LocalRegion();
	virtual bool Init();
protected:
    virtual void DrawChildRegion(int width, int height);
private:
    ImTextureID GetTextureId(TextureName textureName);
    bool DrawLabel(const char* name);
    int fileListWidth;
    int fileListHeight;
};
#endif
