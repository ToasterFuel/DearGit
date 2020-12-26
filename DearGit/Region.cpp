#include "Region.h"
#include "imgui/imgui.h"

bool Region::Init(Window* window, Repo* repo)
{
    this->window = window;
    if(!ChildInit())
        return false;
    SetRepo(repo);
    return true;
}

void Region::DrawRegion(int width, int height)
{
    ImGui::BeginGroup();
    DrawChildRegion(width, height);
    ImGui::EndGroup();
}

void Region::SetRepo(Repo* repo)
{
    this->repo = repo;
    Refresh();
}

void Region::Refresh()
{
    ChildRefresh();
}
