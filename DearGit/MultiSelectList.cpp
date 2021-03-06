#include "MultiSelectList.h"
#include "Window.h"
#include "imgui/imgui.h"

const char* MultiSelectListItemProvider::GetLabel(int index)
{
    if(index < 0 || index >= GetSize())
        return "";
    return ChildGetLabel(index);
}

int MultiSelectListItemProvider::GetSize()
{
    int size = ChildGetSize();
    if(size < 0)
        size = 0;
    return size;
}


MultiSelectList::MultiSelectList(Window* window, MultiSelectListItemProvider* provider): lastSelected(-1), window(window), provider(provider)
{
    ClearSelections();
}

void MultiSelectList::SetData(MultiSelectListItemProvider* provider)
{
    this->provider = provider;
    ClearSelections();
}

bool MultiSelectList::HasSelections()
{
    return !highlightedIndices.empty();
}

void MultiSelectList::ClearSelections()
{
    lastSelected = -1;
    highlightedIndices.clear();
}

std::set<int> MultiSelectList::GetSelectedItems()
{
    return highlightedIndices;
}

bool MultiSelectList::Draw()
{
    bool pressed = false;
    if(provider == nullptr || window == nullptr)
        return pressed;
    for(int i = 0; i < provider->GetSize(); i++)
    {
        bool state = ImGui::Selectable(provider->GetLabel(i), IndexHighlighted(i));
        if(state)
        {
            pressed = true;
            if(window->IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || window->IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
            {
                highlightedIndices.clear();
                if(lastSelected >= 0)
                {
                    int start = i;
                    int end = lastSelected;
                    if(lastSelected < i)
                    {
                        start = lastSelected;
                        end = i;
                    }
                    for(int j = start; j <= end; j++)
                    {
                        highlightedIndices.insert(j);
                    }
                }
                else
                {
                    highlightedIndices.insert(i);
                    lastSelected = i;
                }
            }
            else if(window->IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || window->IsKeyPressed(GLFW_KEY_RIGHT_CONTROL))
            {
                lastSelected = i;
                if (IndexHighlighted(i))
                    highlightedIndices.erase(i);
                else
                    highlightedIndices.insert(i);
            }
            else
            {
                lastSelected = i;
                highlightedIndices.clear();
                highlightedIndices.insert(i);
            }
        }
    }
    return pressed;
}

bool MultiSelectList::IndexHighlighted(int index)
{
    return highlightedIndices.find(index) != highlightedIndices.end();
}
