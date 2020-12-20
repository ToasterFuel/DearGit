#ifndef __REGION_H__
#define __REGION_H__

class Window;

class Region
{
public:
    void DrawRegion(int width, int height);
    bool Init(Window* window);
protected:
	virtual bool ChildInit() = 0;
    virtual void DrawChildRegion(int width, int height) = 0;

    Window* window;
};
#endif