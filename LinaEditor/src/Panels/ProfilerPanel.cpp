/* 
This file is a part of: Lina Engine
https://github.com/inanevin/LinaEngine

Author: Inan Evin
http://www.inanevin.com

Copyright (c) [2018-2020] [Inan Evin]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Panels/ProfilerPanel.hpp"
#include "Widgets/WidgetsUtility.hpp"
#include "Core/Application.hpp"
#include "imgui/imgui.h"

namespace LinaEditor
{
    void ProfilerPanel::Setup()
    {

    }

    void ProfilerPanel::Draw(float frameTime)
    {
        if (m_show)
        {
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
            ImGui::SetNextWindowBgAlpha(1.0f);
            LinaEngine::Application& app = LinaEngine::Application::GetApp();

            WidgetsUtility::IncrementCursorPosY(12);

            if (ImGui::Begin("Profiler", &m_show, flags))
            {
                WidgetsUtility::IncrementCursorPosX(12);

              
                ImGui::End();
            }
        }
    }
}