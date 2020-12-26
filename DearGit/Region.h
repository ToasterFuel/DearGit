#ifndef __REGION_H__
#define __REGION_H__

#include "git/Repo.h"

class Window;

class Region
{
public:
    void DrawRegion(int width, int height);
    bool Init(Window* window, Repo* repo);
    void SetRepo(Repo* repo);
    void Refresh();
protected:
    virtual void ChildRefresh() = 0;
	virtual bool ChildInit() = 0;
    virtual void DrawChildRegion(int width, int height) = 0;

    Repo* repo;
    Window* window;
};
#endif