#ifndef __IM_GUI_HELPERS_H__
#define __IM_GUI_HELPERS_H__

#include "imgui/imgui.h"

namespace ImGui
{
    void PushDisabled();
    void PopDisabled();
    bool ShowTextDialog(const char* dialogName, const char* commandText, const char* label, char* buffer, int bufferSize, bool& confiremed);
}

#endif
