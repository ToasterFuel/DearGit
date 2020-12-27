#include "ImGuiHelpers.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

void ImGui::PushDisabled()
{
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
}

void ImGui::PopDisabled()
{
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
}
