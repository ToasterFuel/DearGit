#include "Region.h"
#include "imgui/imgui.h"

bool Region::Init(Window* window)
{
    this->window = window;
    return ChildInit();
}

void Region::DrawRegion(int width, int height)
{
    ImGui::BeginGroup();
    DrawChildRegion(width, height);
    ImGui::EndGroup();
}