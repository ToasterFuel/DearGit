#include "Region.h"
#include "imgui/imgui.h"

void Region::DrawRegion(int width, int height)
{
    ImGui::BeginGroup();
    DrawChildRegion(width, height);
    ImGui::EndGroup();
}