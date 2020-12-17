#include "LocalRegion.h"
#include "TextureHolder.h"
#include "imgui/imgui.h"
#include "Utils.h"

#define MIN_SIZE  200

LocalRegion::LocalRegion(): fileListWidth(-1), fileListHeight(-1)
{
}

bool LocalRegion::Init()
{
    return true;
}

void LocalRegion::DrawChildRegion(int width, int height)
{
    if(fileListWidth <= 0)
        fileListWidth = width / 3;
    if(fileListHeight <= 0)
        fileListHeight = height / 2;

    char label[128];
    ImGui::BeginGroup();
    {
        ImGui::ImageButton(GetTextureId(TEXTURE_PUSH), ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), 5);
        ImGui::ImageButton(GetTextureId(TEXTURE_PULL), ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), 5);
        ImGui::Button("Push", ImVec2(50, 50));
        ImGui::Button("Pull", ImVec2(50, 50));
        ImGui::Button("Commit", ImVec2(50, 50));
        ImGui::Button("Add", ImVec2(50, 50));
        ImGui::Button("Minus", ImVec2(50, 50));
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
        DrawLabel("Staged Files");
        ImGui::BeginChild("Staged Files", ImVec2(fileListWidth, fileListHeight), true);
        for (int i = 0; i < 100; i++)
        {
            sprintf_s(label, "Staged_%d.txt", i);
            ImGui::Selectable(label, false);
        }
        ImGui::EndChild();

        if (DrawLabel("Unstaged Files"))
            fileListHeight += ImGui::GetIO().MouseDelta.y;
        fileListHeight = Utils::Clamp(MIN_SIZE, height - MIN_SIZE, fileListHeight);

        ImGui::BeginChild("Unstaged Files", ImVec2(fileListWidth, 0), true);
        for (int i = 0; i < 100; i++)
        {
            sprintf_s(label, "Unstaged_%d.txt", i);
            ImGui::Selectable(label, false);
        }
        ImGui::EndChild();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, height));
    if(ImGui::IsItemActive())
        fileListWidth += ImGui::GetIO().MouseDelta.x;
    fileListWidth = Utils::Clamp(MIN_SIZE, width - MIN_SIZE, fileListWidth);
    ImGui::SameLine();
    ImGui::BeginChild("File Viewer", ImVec2(0, 0), true);
    ImGui::EndChild();
}

ImTextureID LocalRegion::GetTextureId(TextureName textureName)
{
    return (ImTextureID)TextureHolder::Instance().GetTextureId(textureName);
}


bool LocalRegion::DrawLabel(const char* name)
{
    ImVec4 backgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
    ImGui::PushStyleColor(ImGuiCol_Button, backgroundColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, backgroundColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, backgroundColor);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(-0.1, .5));
    ImGui::Button(name, ImVec2(fileListWidth, ImGui::GetTextLineHeightWithSpacing()));
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();

    return ImGui::IsItemActive();
}
