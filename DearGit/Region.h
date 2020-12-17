#ifndef __REGION_H__
#define __REGION_H__

class Region
{
public:
    void DrawRegion(int width, int height);
	virtual bool Init() = 0;
protected:
    virtual void DrawChildRegion(int width, int height) = 0;
};
#endif