#include "ImGuiHelpers.h"
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

bool ImGui::ShowTextDialog(const char* dialogName, const char* commandText, const char* textLabel, char* buffer, int bufferSize, bool& confirmed)
{
    bool open = true;
    int buttonSize = 100;
    bool returnValue = false;
    ImGui::OpenPopup(dialogName);
    ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    ImGui::SetNextWindowSizeConstraints(ImVec2(buttonSize * 2.5, buttonSize * 2), ImVec2(FLT_MAX, FLT_MAX));
    if(ImGui::BeginPopupModal(dialogName, &open))
    {
        ImGui::Text(commandText);
        ImGui::InputTextMultiline(textLabel, buffer, bufferSize, ImVec2(-ImGui::GetStyle().FramePadding.x / 2, -ImGui::GetFrameHeightWithSpacing()),
            ImGuiInputTextFlags_AutoSelectAll |
            ImGuiInputTextFlags_AllowTabInput);

        ImGui::SetCursorPosX(ImGui::GetItemRectSize().x - (buttonSize * 2));
        if(ImGui::Button("Cancel", ImVec2(buttonSize, 0)))
        {
            ImGui::CloseCurrentPopup();
            confirmed = false;
            returnValue = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("OK", ImVec2(buttonSize, 0)))
        {
            ImGui::CloseCurrentPopup();
            confirmed = true;
            returnValue = true;
        }
        ImGui::EndPopup();
    }

    if(!open)
    {
        confirmed = false;
        returnValue = true;
    }

    return returnValue;
}

bool ImGui::IsItemHovered(float threshold)
{
    return ImGui::IsItemHovered() && GImGui->HoveredIdTimer > threshold;
}
